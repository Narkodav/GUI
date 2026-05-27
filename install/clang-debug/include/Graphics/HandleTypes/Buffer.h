#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Image.h"

namespace Graphics
{
    class DeviceRef;

    class BufferRef : public BaseComponent<VkBuffer, BufferRef>
    {
        using Base = BaseComponent<VkBuffer, BufferRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "Buffer";

        MemoryRequirements getMemoryRequirements(const DeviceFunctionTable& functions, const DeviceRef& device) const;
    };
    GRAPHICS_ASSERT_HANDLE_REFERENCE_PROPERTIES(BufferRef);

    class BufferCreateInfo : public StructBase<VkBufferCreateInfo, BufferCreateInfo>
    {
        using Base = StructBase<VkBufferCreateInfo, BufferCreateInfo>;
    public:
        using Base::Base;
        BufferCreateInfo(DeviceSize byteSize, Flags::BufferUsage usageFlags,
            SharingMode sharingMode = SharingMode::Exclusive) : Base()
        {
            this->size = byteSize;
            this->usage = usageFlags;
            this->sharingMode = static_cast<VkSharingMode>(sharingMode);
            this->flags = 0;
            this->queueFamilyIndexCount = 0;
            this->pQueueFamilyIndices = nullptr;
        }
        BufferCreateInfo& setSize(DeviceSize byteSize) {
            this->size = byteSize;
            return *this;
        }
        BufferCreateInfo& setUsage(Flags::BufferUsage usageFlags) {
            this->usage = usageFlags;
            return *this;
        }
        BufferCreateInfo& setSharingMode(SharingMode sharingMode) {
            this->sharingMode = static_cast<VkSharingMode>(sharingMode);
            return *this;
        }
        BufferCreateInfo& setQueueFamilyIndices(std::span<const uint32_t> queueFamilyIndices) {
            this->queueFamilyIndexCount = queueFamilyIndices.size();
            this->pQueueFamilyIndices = queueFamilyIndices.data();
            return *this;
        }

        DeviceSize getSize() const { return this->size; }
        Flags::BufferUsage getUsage() const { return this->usage; }
        SharingMode getSharingMode() const { return static_cast<SharingMode>(this->sharingMode); }
        std::span<const uint32_t> getQueueFamilyIndices() const {
            return std::span<const uint32_t>(this->pQueueFamilyIndices, this->queueFamilyIndexCount);
        }
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::BufferCreateInfo, VkBufferCreateInfo);

    class BufferMemoryBarrier : public StructBase<VkBufferMemoryBarrier, BufferMemoryBarrier>
    {
        using Base = StructBase<VkBufferMemoryBarrier, BufferMemoryBarrier>;
    public:
        using Base::Base;
        BufferMemoryBarrier(const BufferRef& buffer, Flags::Access srcAccessMask, Flags::Access dstAccessMask,
            uint32_t srcQueueFamilyIndex, uint32_t dstQueueFamilyIndex,
            DeviceSize offset, DeviceSize size) : Base()
        {
            this->srcAccessMask = srcAccessMask;
            this->dstAccessMask = dstAccessMask;
            this->srcQueueFamilyIndex = srcQueueFamilyIndex;
            this->dstQueueFamilyIndex = dstQueueFamilyIndex;
            this->buffer = buffer;
            this->offset = offset;
            this->size = size;
        }
        BufferMemoryBarrier& setSrcAccessMask(Flags::Access srcAccessMask) {
            this->srcAccessMask = srcAccessMask;
            return *this;
        }
        BufferMemoryBarrier& setDstAccessMask(Flags::Access dstAccessMask) {
            this->dstAccessMask = dstAccessMask;
            return *this;
        }
        BufferMemoryBarrier& setSrcQueueFamilyIndex(uint32_t srcQueueFamilyIndex) {
            this->srcQueueFamilyIndex = srcQueueFamilyIndex;
            return *this;
        }
        BufferMemoryBarrier& setDstQueueFamilyIndex(uint32_t dstQueueFamilyIndex) {
            this->dstQueueFamilyIndex = dstQueueFamilyIndex;
            return *this;
        }
        BufferMemoryBarrier& setBuffer(const BufferRef& buffer) {
            this->buffer = buffer;
            return *this;
        }
        BufferMemoryBarrier& setOffset(DeviceSize offset) {
            this->offset = offset;
            return *this;
        }
        BufferMemoryBarrier& setSize(DeviceSize size) {
            this->size = size;
            return *this;
        }
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::BufferMemoryBarrier, VkBufferMemoryBarrier);

    class BufferImageCopy : public StructBase<VkBufferImageCopy, BufferImageCopy>
    {
        using Base = StructBase<VkBufferImageCopy, BufferImageCopy>;
    public:
        using Base::Base;
        BufferImageCopy(DeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight,
            ImageSubresourceLayers imageSubresource, Offset3D imageOffset, Extent3D imageExtent) : Base()
        {
            this->bufferOffset = bufferOffset;
            this->bufferRowLength = bufferRowLength;
            this->bufferImageHeight = bufferImageHeight;
            this->imageSubresource = imageSubresource;
            this->imageOffset = imageOffset;
            this->imageExtent = imageExtent;
        }
        BufferImageCopy& setBufferOffset(DeviceSize bufferOffset) {
            this->bufferOffset = bufferOffset;
            return *this;
        }
        BufferImageCopy& setBufferRowLength(uint32_t bufferRowLength) {
            this->bufferRowLength = bufferRowLength;
            return *this;
        }
        BufferImageCopy& setBufferImageHeight(uint32_t bufferImageHeight) {
            this->bufferImageHeight = bufferImageHeight;
            return *this;
        }
        BufferImageCopy& setImageSubresource(const ImageSubresourceLayers& imageSubresource) {
            this->imageSubresource = imageSubresource;
            return *this;
        }
        BufferImageCopy& setImageOffset(Offset3D imageOffset) {
            this->imageOffset = imageOffset;
            return *this;
        }
        BufferImageCopy& setImageExtent(Extent3D imageExtent) {
            this->imageExtent = imageExtent;
            return *this;
        }
        ImageSubresourceLayers& getImageSubresource() {
            return *reinterpret_cast<ImageSubresourceLayers*>(&this->imageSubresource);
        }
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::BufferImageCopy, VkBufferImageCopy);

    class BufferViewRef : public BaseComponent<VkBufferView, BufferViewRef>
    {
        using Base = BaseComponent<VkBufferView, BufferViewRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "Buffer::View";
    };
    GRAPHICS_ASSERT_HANDLE_REFERENCE_PROPERTIES(BufferViewRef);

    class BufferViewCreateInfo : public StructBase<VkBufferViewCreateInfo, BufferViewCreateInfo>
    {
        using Base = StructBase<VkBufferViewCreateInfo, BufferViewCreateInfo>;
    public:
        using Base::Base;
        BufferViewCreateInfo(const BufferRef& buffer, Format format, DeviceSize offset, DeviceSize range,
            Flags::BufferViewCreate flags = Flags::BufferViewCreate::Bits::None) : Base()
        {
            this->buffer = buffer;
            this->format = convertCEnum(format);
            this->offset = offset;
            this->range = range;
            this->flags = flags;
        }
        BufferViewCreateInfo& setBuffer(const BufferRef& buffer) {
            this->buffer = buffer;
            return *this;
        }
        BufferViewCreateInfo& setFormat(Format format) {
            this->format = convertCEnum(format);
            return *this;
        }
        BufferViewCreateInfo& setOffset(DeviceSize offset) {
            this->offset = offset;
            return *this;
        }
        BufferViewCreateInfo& setRange(DeviceSize range) {
            this->range = range;
            return *this;
        }
        BufferViewCreateInfo& setFlags(Flags::BufferViewCreate flags) {
            this->flags = flags;
            return *this;
        }
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::BufferViewCreateInfo, VkBufferViewCreateInfo);

    class BufferView : public VerificatorComponent<VkBufferView, BufferViewRef>
    {
        using Base = VerificatorComponent<VkBufferView, BufferViewRef>;
    public:
        using Base::Base;

        void create(const DeviceFunctionTable& functions, const DeviceRef& device, const BufferViewCreateInfo& createInfo);
        void destroy(const DeviceFunctionTable& functions, const DeviceRef& device);
    };
    GRAPHICS_ASSERT_HANDLE_PROPERTIES(BufferView);

    class Buffer : public VerificatorComponent<VkBuffer, BufferRef>
    {
        using Base = VerificatorComponent<VkBuffer, BufferRef>;
    public:
        using Base::Base;

        void create(const DeviceFunctionTable& functions, const DeviceRef& device, const BufferCreateInfo& createInfo);
        void destroy(const DeviceFunctionTable& functions, const DeviceRef& device);
    };
    GRAPHICS_ASSERT_HANDLE_PROPERTIES(Buffer);
}

