#pragma once
#include "Graphics/Common.h"
#include "Graphics/Structs.h"
#include "Graphics/DeviceFunctionTable.h"
#include "Device.h"
#include "CommandBuffer.h"
#include "Queue.h"

namespace Graphics
{
	struct DataTransferInfo {
		const void* sourceData;
		size_t      sourceOffset;
		BufferRef   destinationBuffer;
		size_t      destinationOffset;
		size_t      byteSize;

		DataTransferInfo(const void* sourceData, size_t sourceOffset, BufferRef destinationBuffer,
			size_t destinationOffset, size_t byteSize) : sourceData(sourceData), sourceOffset(sourceOffset), 
			destinationBuffer(destinationBuffer), destinationOffset(destinationOffset), byteSize(byteSize) {}

		template <typename T>
		DataTransferInfo(std::span<const T> sourceData, size_t sourceOffset, BufferRef destinationBuffer, 
			size_t destinationOffset = 0) : sourceData(static_cast<const void*>(sourceData.data())), 
			sourceOffset(sourceOffset), destinationBuffer(destinationBuffer), destinationOffset(destinationOffset), 
			byteSize(sourceData.size() * sizeof(T)) {}

		template <typename T>
		DataTransferInfo(std::span<const T> sourceData, BufferRef destinationBuffer, size_t destinationOffset = 0) 
			: sourceData(static_cast<const void*>(sourceData.data())), sourceOffset(0), 
			destinationBuffer(destinationBuffer), destinationOffset(destinationOffset), 
			byteSize(sourceData.size() * sizeof(T)) {}

		DataTransferInfo& setSourceData(const void* sourceData) {
			this->sourceData = sourceData;
			return *this;
		}

		DataTransferInfo& setSourceOffset(size_t sourceOffset) {
			this->sourceOffset = sourceOffset;
			return *this;
		}

		DataTransferInfo& setDestinationBuffer(BufferRef destinationBuffer) {
			this->destinationBuffer = destinationBuffer;
			return *this;
		}

		DataTransferInfo& setDestinationOffset(size_t destinationOffset) {
			this->destinationOffset = destinationOffset;
			return *this;
		}

		DataTransferInfo& setByteSize(size_t byteSize) {
			this->byteSize = byteSize;
			return *this;
		}

		const void* getSourceData() const { return this->sourceData; }
		size_t getSourceOffset() const { return this->sourceOffset; }
		BufferRef getDestinationBuffer() const { return this->destinationBuffer; }
		size_t getDestinationOffset() const { return this->destinationOffset; }
		size_t getByteSize() const { return this->byteSize; }
	};


	class CommandPoolRef : public BaseComponent<VkCommandPool, CommandPoolRef>
	{
		using Base = BaseComponent<VkCommandPool, CommandPoolRef>;
	public:
		using Base::Base;
		static inline const std::string s_typeName = "CommandPool";

		CommandBuffer allocateCommandBuffer(const DeviceFunctionTable& functions, DeviceRef device,
			CommandBufferLevel level = CommandBufferLevel::Primary);

		void freeCommandBuffer(const DeviceFunctionTable& functions, DeviceRef device,
			CommandBuffer buffer);

		std::vector<CommandBuffer> allocateCommandBuffers(const DeviceFunctionTable& functions, DeviceRef device,
			size_t count, CommandBufferLevel level = CommandBufferLevel::Primary);

		void freeCommandBuffers(const DeviceFunctionTable& functions, DeviceRef device,
			std::span<const CommandBuffer> buffer);

		void reset(const DeviceFunctionTable& functions, DeviceRef device,
			Flags::CommandPoolReset flags = Flags::CommandPoolReset::Bits::ReleaseResources);

		template<typename Func>
		Result oneTimeSubmit(const DeviceFunctionTable& functions, DeviceRef device,
			Queue submitQueue, Func&& func)
		{
			GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
			auto temporary = allocateCommandBuffer(functions, device);

			auto result = temporary.record(functions, { Flags::CommandBufferUsage::Bits::OneTimeSubmit },
				std::forward<Func>(func));
			if (result != Result::Success)
				return result;

			QueueSubmitInfo submitInfo;
			submitInfo.setCommandBuffer(temporary);
			submitQueue.submit(functions, submitInfo, nullptr);
			submitQueue.waitIdle(functions);
			freeCommandBuffer(functions, device, temporary);
			return Result::Success;
		};

		Result oneTimeDataTransfer(const DeviceFunctionTable& functions, DeviceRef device,
			Queue submitQueue, std::span<const DataTransferInfo> transferInfos, 
			BufferRef stagingBuffer, MemoryMapping& stagingMapping);

		Result oneTimeDataTransfer(const DeviceFunctionTable& functions, DeviceRef device,
			Queue submitQueue, std::span<const DataTransferInfo> transferInfos, BufferRef stagingBuffer, 
			MemoryMapping& stagingMapping, size_t stagingSize);

		template<typename Target, typename Struct, typename Assignment>
		Result oneTimeDataTransfer(const DeviceFunctionTable& functions, DeviceRef device,
			Queue submitQueue, std::span<const Struct> data, BufferRef dstBuffer,
			BufferRef stagingBuffer, size_t dstOffset, MemoryMapping& stagingMapping, Assignment&& assign)
		{
			return this->oneTimeSubmit(functions, device, submitQueue,
        		[&](Result result, CommandBuffer commandBuffer) {
				if(result != Result::Success) return result;
				auto* stagingPtr = stagingMapping.get<Target>();
				for(size_t i = 0; i < data.size(); ++i) assign(stagingPtr[i], data[i]);
				commandBuffer.copyBuffer(functions, stagingBuffer, dstBuffer,
					{ 0, dstOffset, data.size() * sizeof(Target) });
				return Result::Success;
			});
		}

	};

	class CommandBufferAllocateInfo : public StructBase<VkCommandBufferAllocateInfo, CommandBufferAllocateInfo>
	{
		using Base = StructBase<VkCommandBufferAllocateInfo, CommandBufferAllocateInfo>;
	public:
		using Base::Base;

		CommandBufferAllocateInfo(const CommandPoolRef& commandPool,
			CommandBufferLevel level = CommandBufferLevel::Primary,
			uint32_t commandBufferCount = 1) : Base() {
			this->commandPool = commandPool.getHandle();
			this->level = convertCEnum(level);
			this->commandBufferCount = commandBufferCount;
		}
		CommandBufferAllocateInfo& setCommandPool(const CommandPoolRef& commandPool) {
			this->commandPool = commandPool.getHandle();
			return *this;
		}
		CommandBufferAllocateInfo& setLevel(CommandBufferLevel level) {
			this->level = convertCEnum(level);
			return *this;
		}
		CommandBufferAllocateInfo& setCommandBufferCount(uint32_t commandBufferCount) {
			this->commandBufferCount = commandBufferCount;
			return *this;
		}
		CommandPoolRef getCommandPool() const { return CommandPoolRef(this->commandPool); }
		CommandBufferLevel getLevel() const { return convertCEnum(this->level); }
		uint32_t getCommandBufferCount() const { return this->commandBufferCount; }
	};

	class CommandPoolCreateInfo : public StructBase<VkCommandPoolCreateInfo, CommandPoolCreateInfo>
	{
		using Base = StructBase<VkCommandPoolCreateInfo, CommandPoolCreateInfo>;
	public:
		using Base::Base;
		CommandPoolCreateInfo(uint32_t queueFamilyIndex,
			Flags::CommandPoolCreate flags = Flags::CommandPoolCreate::Bits::ResetCommandBuffer) : Base()
		{
			this->queueFamilyIndex = queueFamilyIndex;
			this->flags = flags;
		}
		CommandPoolCreateInfo& setQueueFamilyIndex(uint32_t queueFamilyIndex) {
			this->queueFamilyIndex = queueFamilyIndex;
			return *this;
		}
		CommandPoolCreateInfo& setFlags(Flags::CommandPoolCreate flags) {
			this->flags = flags;
			return *this;
		}
		uint32_t getQueueFamilyIndex() const { return this->queueFamilyIndex; }
		Flags::CommandPoolCreate getFlags() const { return this->flags; }
	};

	class CommandPool : public VerificatorComponent<VkCommandPool, CommandPoolRef> {
		using Base = VerificatorComponent<VkCommandPool, CommandPoolRef>;
	public:
		using Base::Base;

		void create(const DeviceFunctionTable& functions, DeviceRef device, const CommandPoolCreateInfo& createInfo);
		void destroy(const DeviceFunctionTable& functions, DeviceRef device);

		static std::vector<CommandBuffer> allocateCommandBuffers(const DeviceFunctionTable& functions, 
			DeviceRef device, const CommandBufferAllocateInfo& allocInfo);
	};
}



