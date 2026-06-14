#pragma once
#include "Graphics/Common.h"
#include "Graphics/Flags.h"
#include "Graphics/HandleTypes/Device.h"
#include "Graphics/HandleTypes/Instance.h"
#include "Graphics/HandleTypes/Memory.h"
#include "Graphics/HandleTypes/Buffer.h"
#include "Graphics/Utility//Utility.h"

#include "Memory/ExternalMetadataAllocators/BuddyAllocator.h"
#include "Memory/ExternalMetadataAllocators/ListAllocator.h"

#include <vector>
#include <list>

namespace Allocators = Memory::ExternalMetadataAllocators;

namespace Graphics::MemoryManagement
{
	class MemoryPool
	{
	public:
		struct MemoryRegion {
			uint32_t offset;
			uint32_t size;
		};

		struct Allocation {
			MemoryRegion region;
			uint32_t bufferIndex;

			struct Hash
			{
				size_t operator()(const Allocation& allocation) const {
					return std::hash<uint32_t>()(allocation.bufferIndex) ^ std::hash<uint32_t>()(allocation.region.offset);
				}
			};

			struct EqualTo
			{
				constexpr bool operator()(const Allocation& left, const Allocation& right) const {
					return left.region.offset == right.region.offset
					&& left.region.size == right.region.size
					&& left.bufferIndex == right.bufferIndex;
				}
			};

			static Allocation getEmptyAllocation() {
				return Allocation{ MemoryRegion{ 0, 0 }, 0 };
			};

			bool operator==(const Allocation& other) {
				return region.offset == other.region.offset
					&& region.size == other.region.size
					&& bufferIndex == other.bufferIndex;
			}
		};

		using AllocationSet = std::unordered_set<Allocation, typename Allocation::Hash, typename Allocation::EqualTo>;

	private:
		struct MemoryChunk {
			Buffer buffer;
			Memory memory;
			Allocators::BuddyAllocatorBase alloc;
			//Allocators::ListAllocatorBase alloc;
			uint32_t allocatedSize;
		};

		std::vector<MemoryChunk> m_memoryChunks;

		uint32_t m_chunkCapacity = 0;
		uint32_t m_alignment = 0;
		
		BufferCreateInfo m_bufferInfo;
		PhysicalDeviceMemoryProperties m_deviceMemoryProps;

		// memory type is reasonably guaranteed to be identical for identical alloc infos,
		// doesnt depend on size or alignment
		uint32_t m_memoryTypeIndex; 
		uint32_t m_memoryTypeBits;

	public:

		MemoryPool() = default;

		MemoryPool(MemoryPool&& other) noexcept
		{
			m_memoryChunks = std::exchange(other.m_memoryChunks, {});
			m_chunkCapacity = std::exchange(other.m_chunkCapacity, 0);
			m_bufferInfo = std::exchange(other.m_bufferInfo, {});
			m_deviceMemoryProps = std::exchange(other.m_deviceMemoryProps, {});
			m_memoryTypeIndex = std::exchange(other.m_memoryTypeIndex, 0);
		}

		MemoryPool& operator=(MemoryPool&& other)
		{
			if (this == &other)
				return *this;

			m_memoryChunks = std::exchange(other.m_memoryChunks, {});
			m_chunkCapacity = std::exchange(other.m_chunkCapacity, 0);
			m_bufferInfo = std::exchange(other.m_bufferInfo, {});
			m_deviceMemoryProps = std::exchange(other.m_deviceMemoryProps, {});
			m_memoryTypeIndex = std::exchange(other.m_memoryTypeIndex, 0);

			return *this;
		}

		MemoryPool(const MemoryPool&) noexcept = delete;
		MemoryPool& operator=(const MemoryPool&) noexcept = delete;

		~MemoryPool() { GRAPHICS_VERIFY(m_memoryChunks.empty(), "MemoryPool was dont destroyed"); };

		template<typename Func>
		void create(const DeviceFunctionTable& functions, DeviceRef device, PhysicalDeviceMemoryProperties deviceMemoryProps,
			uint32_t memoryChunkCapacity, uint32_t allocationAlignment, Flags::BufferUsage usageFlags, Flags::MemoryProperty requiredProperties, 
			Flags::MemoryProperty forbiddenProperties, SharingMode sharingMode, 
			Func&& onBufferAlloc = [](Memory&, Buffer&, size_t) {}) {
			
			m_deviceMemoryProps = deviceMemoryProps;
			m_alignment = allocationAlignment;
			m_chunkCapacity = Allocators::BuddyAllocatorBase::alignHeapSize(memoryChunkCapacity, m_alignment);
			
			//m_chunkCapacity = memoryChunkCapacity;
			m_memoryChunks.emplace_back();
			auto& chunk = m_memoryChunks.back();

			m_bufferInfo = { m_chunkCapacity, usageFlags, sharingMode };

			chunk.buffer.create(functions, device, m_bufferInfo);
			auto memoryRequirements = chunk.buffer.getMemoryRequirements(functions, device);
			m_memoryTypeBits = memoryRequirements.getMemoryTypeBits();
			m_memoryTypeIndex = Utility::findMemoryTypeFirstFit(deviceMemoryProps,
					m_memoryTypeBits, requiredProperties, forbiddenProperties);

			chunk.memory.create(functions, device, { memoryRequirements.getSize(), m_memoryTypeIndex });
			chunk.memory.bindBuffer(functions, device, chunk.buffer);

			onBufferAlloc(chunk.memory, chunk.buffer, 0);

			chunk.alloc.assign(0, m_chunkCapacity, m_alignment);
			chunk.allocatedSize = 0;
		}

		template<typename Func>
		Allocation allocate(const DeviceFunctionTable& functions, DeviceRef device, size_t size, 
			Func&& onBufferAlloc = [](MemoryRef, BufferRef, size_t) {})
		{
			if (size > m_chunkCapacity)
				throw std::runtime_error("Allocation size exceeds chunk capacity");

			for (size_t i = 0; i < m_memoryChunks.size(); ++i)
			{
				auto ptr = m_memoryChunks[i].alloc.allocate(size);
				if(ptr != std::numeric_limits<uintptr_t>::max())
					return {static_cast<uint32_t>(ptr), static_cast<uint32_t>(size), static_cast<uint32_t>(i)};
			}
			addBuffer(functions, device, std::forward<Func>(onBufferAlloc));
			auto ptr = m_memoryChunks.back().alloc.allocate(size);
			m_memoryChunks.back().alloc.debugPrint();
			return {static_cast<uint32_t>(ptr), static_cast<uint32_t>(size), 
				static_cast<uint32_t>(m_memoryChunks.size() - 1)};
		}

		void free(Allocation& allocation)
		{
			if(allocation.region.size == 0) return;
			m_memoryChunks[allocation.bufferIndex].alloc.deallocate(allocation.region.offset);
		}

		BufferRef getBuffer(size_t chunkIndex) const { return m_memoryChunks[chunkIndex].buffer; };
		MemoryRef getMemory(size_t chunkIndex) const { return m_memoryChunks[chunkIndex].memory; };

		size_t getMemoryChunkCount() const { return m_memoryChunks.size(); }

		// const auto& getFreeRegions(size_t chunkIndex) const { return m_freeRegions[chunkIndex];  };
		// const auto& getAllocations(size_t chunkIndex) const { return m_allocations[chunkIndex]; };
		// const auto& getAllFreeRegions() const { return m_freeRegions; };
		// const auto& getAllAllocations() const { return m_allocations; };

		size_t getChunkSize() const { return m_chunkCapacity; };
		size_t getChunkAllocatedSize(size_t chunkIndex) const { return m_memoryChunks[chunkIndex].allocatedSize; };
		size_t getChunkFreeSize(size_t chunkIndex) const { return m_chunkCapacity - m_memoryChunks[chunkIndex].allocatedSize; };

		void destroy(const DeviceFunctionTable& functions, DeviceRef device) {
			if (m_memoryChunks.empty())
				return;

			for (auto& chunk : m_memoryChunks)
			{
				chunk.buffer.destroy(functions, device);
				chunk.memory.destroy(functions, device);
			}
			m_memoryChunks.clear();
		}

	private:

		template<typename Func>
		void addBuffer(const DeviceFunctionTable& functions, DeviceRef device, 
			Func&& onBufferAlloc = [](Memory&, Buffer&, size_t) {})
		{
			m_memoryChunks.push_back(MemoryChunk{});
			auto& chunk = m_memoryChunks.back();

			chunk.buffer.create(functions, device, m_bufferInfo);
			auto memoryRequirements = chunk.buffer.getMemoryRequirements(functions, device);
			GRAPHICS_VERIFY(m_memoryTypeBits & (static_cast<size_t>(1) << m_memoryTypeIndex), 
			"Required memory type doesnt match cached memory type");

			chunk.memory.create(functions, device, { memoryRequirements.getSize(), m_memoryTypeIndex });
			chunk.memory.bindBuffer(functions, device, chunk.buffer);
			chunk.alloc.assign(0, m_chunkCapacity, m_alignment);
			chunk.allocatedSize = 0;
			onBufferAlloc(chunk.memory, chunk.buffer, m_memoryChunks.size() - 1);
		}
	};
}
