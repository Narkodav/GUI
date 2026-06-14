#pragma once
#include "Graphics/Common.h"
#include "Graphics/Structs.h"
#include "Graphics/DeviceFunctionTable.h"
#include "Graphics/HandleTypes/Device.h"

namespace Graphics
{
    class DeviceRef;
	class BufferRef;
	class ImageRef;

    class MemoryRef : public BaseComponent<VkDeviceMemory, MemoryRef>
    {
        using Base = BaseComponent<VkDeviceMemory, MemoryRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "Memory";

        void bindBuffer(const DeviceFunctionTable& functions, DeviceRef device,
            BufferRef buffer, size_t offset = 0);
        void bindImage(const DeviceFunctionTable& functions, DeviceRef device,
            ImageRef image, size_t offset = 0);
    };

    class MemoryType : public StructBase<VkMemoryType, MemoryType>
    {
        using Base = StructBase<VkMemoryType, MemoryType>;
    public:
        using Base::Base;
        Flags::MemoryProperty getPropertyFlags() const {
            return static_cast<Flags::MemoryProperty>(this->propertyFlags);
        };
        uint32_t getHeapIndex() const {
            return this->heapIndex;
        };
    };

    class MemoryHeap : public StructBase<VkMemoryHeap, MemoryHeap>
    {
        using Base = StructBase<VkMemoryHeap, MemoryHeap>;
    public:
        using Base::Base;
        Flags::MemoryHeap getFlags() const {
            return this->flags;
        };
        VkDeviceSize getSize() const {
            return this->size;
        };
    };

    class PhysicalDeviceMemoryProperties : public StructBase<VkPhysicalDeviceMemoryProperties, PhysicalDeviceMemoryProperties>
    {
        using Base = StructBase<VkPhysicalDeviceMemoryProperties, PhysicalDeviceMemoryProperties>;
    public:
        using Base::Base;

        uint32_t getMemoryTypeCount() const {
            return memoryTypeCount;
        };

        uint32_t getMemoryHeapCount() const {
            return memoryHeapCount;
        };

        std::span<const MemoryType> getMemoryTypes() const {
            return std::span<const MemoryType>(
                reinterpret_cast<const MemoryType*>(memoryTypes), memoryTypeCount);
        };

        std::span<const MemoryHeap> getMemoryHeaps() const {
            return std::span<const MemoryHeap>(
                reinterpret_cast<const MemoryHeap*>(memoryHeaps), memoryHeapCount);
        };
    };

    class MemoryAllocateInfo : public StructBase<VkMemoryAllocateInfo, MemoryAllocateInfo>
    {
        using Base = StructBase<VkMemoryAllocateInfo, MemoryAllocateInfo>;
    public:
        using Base::Base;
        MemoryAllocateInfo(size_t allocationSize, uint32_t memoryTypeIndex) : Base()
        {
            this->allocationSize = static_cast<VkDeviceSize>(allocationSize);
            this->memoryTypeIndex = memoryTypeIndex;
        }
        MemoryAllocateInfo& setAllocationSize(size_t allocationSize) {
            this->allocationSize = static_cast<VkDeviceSize>(allocationSize);
            return *this;
        }
        MemoryAllocateInfo& setMemoryTypeIndex(uint32_t memoryTypeIndex) {
            this->memoryTypeIndex = memoryTypeIndex;
            return *this;
        }
        size_t getAllocationSize() const {
            return static_cast<size_t>(this->allocationSize);
        }
        uint32_t getMemoryTypeIndex() const {
            return this->memoryTypeIndex;
        }
    };

    class MemoryBarrier : public StructBase<VkMemoryBarrier, MemoryBarrier>
    {
        using Base = StructBase<VkMemoryBarrier, MemoryBarrier>;
    public:
        using Base::Base;
        MemoryBarrier(Flags::Access srcAccessMask, Flags::Access dstAccessMask) : Base() {
            this->srcAccessMask = srcAccessMask;
            this->dstAccessMask = dstAccessMask;
        }
        MemoryBarrier& setSrcAccessMask(Flags::Access srcAccessMask) {
            this->srcAccessMask = srcAccessMask;
            return *this;
        }
        MemoryBarrier& setDstAccessMask(Flags::Access dstAccessMask) {
            this->dstAccessMask = dstAccessMask;
            return *this;
        }
    };

    class MemoryMapping {
        friend class Memory;
    private:
        void* m_mapping = nullptr;
        MemoryMapping(void* mapping) :
            m_mapping(mapping) {
        };

        void invalidate() {
            m_mapping = nullptr;
        }

    public:
        static inline const DeviceSize s_wholeSize = VK_WHOLE_SIZE;

        MemoryMapping() = default;
        MemoryMapping(MemoryMapping&&) = default;
        MemoryMapping& operator=(MemoryMapping&& other)
        {
            if (this == &other)
                return *this;
            GRAPHICS_VERIFY(!isValid(), "Mapping was not unmapped before being moved to");
            m_mapping = std::exchange(other.m_mapping, nullptr);
            return *this;
        }
        MemoryMapping(const MemoryMapping&) = delete;
        MemoryMapping& operator=(const MemoryMapping&) = delete;
        ~MemoryMapping() {
            GRAPHICS_VERIFY(!isValid(), "Mapping was not unmapped before being destroyed");
        }

        void unmap(const DeviceFunctionTable& functions, const DeviceRef& device, MemoryRef memory) {
            GRAPHICS_VERIFY(isValid(), "Mapping was not mapped before being unmapped");
            functions.execute<DeviceFunction::UnmapMemory>(device.getHandle(), memory.getHandle());
            m_mapping = nullptr;
        }

        bool isValid() const {
            return m_mapping != nullptr;
        }

        void* get(size_t offset = 0) { return static_cast<std::byte*>(m_mapping) + offset; }
        const void* get(size_t offset = 0) const { return static_cast<const std::byte*>(m_mapping) + offset; }

        template<typename T>
        std::span<T> get(size_t offset, size_t size)
        {
            return std::span<T>(reinterpret_cast<T*>(static_cast<std::byte*>(m_mapping) + offset), size);
        }

        template<typename T>
        const std::span<const T> get(size_t offset, size_t size) const
        {
            return std::span<const T>(reinterpret_cast<const T*>(static_cast<const std::byte*>(m_mapping) + offset), size);
        }

        template<typename T = void>
        T* get(size_t offset = 0)
        {
            return reinterpret_cast<T*>(static_cast<std::byte*>(m_mapping) + offset);
        }

        template<typename T = void>
        const T* get(size_t offset = 0) const
        {
            return reinterpret_cast<const T*>(static_cast<const std::byte*>(m_mapping) + offset);
        }
    };

    class Memory : public VerificatorComponent<VkDeviceMemory, MemoryRef>
    {
        using Base = VerificatorComponent<VkDeviceMemory, MemoryRef>;
    public:
        using Base::Base;
        
		static inline const size_t s_maxMemoryTypes = VK_MAX_MEMORY_TYPES;
        static inline const size_t s_maxMemoryHeaps = VK_MAX_MEMORY_HEAPS;        

        void create(const DeviceFunctionTable& functions, DeviceRef device,
            const MemoryAllocateInfo& allocInfo);

        void destroy(const DeviceFunctionTable& functions, DeviceRef device);

        MemoryMapping map(const DeviceFunctionTable& functions, DeviceRef device,
            DeviceSize offset = 0, DeviceSize size = MemoryMapping::s_wholeSize,
            Flags::MemoryMap flags = Flags::MemoryMap::Bits::None);

        void unmap(const DeviceFunctionTable& functions, DeviceRef device, MemoryMapping& mapping);
    };
}

