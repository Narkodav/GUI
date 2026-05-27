#pragma once
#include "Graphics/Common.h"
#include "Graphics/Structs.h"
#include "Graphics/Enums.h"
#include "Graphics/DeviceFunctionTable.h"

namespace Graphics
{
    class SwapChain;
    class DeviceRef;

    class ComponentMapping : public StructBase<VkComponentMapping, ComponentMapping>
    {
		using Base = StructBase<VkComponentMapping, ComponentMapping>;
        public:
        using Base::Base;
        ComponentMapping(ComponentSwizzle r = ComponentSwizzle::Identity,
            ComponentSwizzle g = ComponentSwizzle::Identity,
            ComponentSwizzle b = ComponentSwizzle::Identity,
            ComponentSwizzle a = ComponentSwizzle::Identity) : Base() {
            this->r = convertCEnum(r);
            this->g = convertCEnum(g);
            this->b = convertCEnum(b);
            this->a = convertCEnum(a);
        }
        ComponentMapping& setR(ComponentSwizzle r) {
            this->r = convertCEnum(r);
            return *this;
        }
        ComponentMapping& setG(ComponentSwizzle g) {
            this->g = convertCEnum(g);
            return *this;
        }
        ComponentMapping& setB(ComponentSwizzle b) {
            this->b = convertCEnum(b);
            return *this;
        }
        ComponentMapping& setA(ComponentSwizzle a) {
            this->a = convertCEnum(a);
            return *this;
        }
	};

    class ImageRef : public BaseComponent<VkImage, ImageRef>
    {
        using Base = BaseComponent<VkImage, ImageRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "Image";

        MemoryRequirements getMemoryRequirements(const DeviceFunctionTable& functions, 
            DeviceRef device) const;
    };

    class ImageCreateInfo : public StructBase<VkImageCreateInfo, ImageCreateInfo>
    {
        using Base = StructBase<VkImageCreateInfo, ImageCreateInfo>;
    public:
        using Base::Base;

        ImageCreateInfo(ImageType imageType = ImageType::T2D,
            Format format = Format::R8G8B8A8Srgb,
            const Extent3D& extent = { 1, 1, 1 },
            uint32_t mipLevels = 1,
            uint32_t arrayLayers = 1,
            Flags::SampleCount::Bits samples = Flags::SampleCount::Bits::SC1,
            ImageTiling tiling = ImageTiling::Optimal,
            Flags::ImageUsage usage = Flags::ImageUsage::Bits::TransferDst |
            Flags::ImageUsage::Bits::Sampled |
            Flags::ImageUsage::Bits::TransferSrc,
            SharingMode sharingMode = SharingMode::Exclusive,
            std::span<const uint32_t> queueFamilyIndices = {},
            ImageLayout initialLayout = ImageLayout::Undefined) : Base()
        {
            this->imageType = convertCEnum(imageType);
            this->format = convertCEnum(format);
            this->extent = extent.getStruct();
            this->mipLevels = mipLevels;
            this->arrayLayers = arrayLayers;
            this->samples = Flags::convertCBit(samples);
            this->tiling = convertCEnum(tiling);
            this->usage = usage;
            this->sharingMode = convertCEnum(sharingMode);
            this->queueFamilyIndexCount = queueFamilyIndices.size();
            this->pQueueFamilyIndices = queueFamilyIndices.data();
            this->initialLayout = convertCEnum(initialLayout);
        }
        ImageCreateInfo& setImageType(ImageType imageType) {
            this->imageType = convertCEnum(imageType);
            return *this;
        }
        ImageCreateInfo& setFormat(Format format) {
            this->format = convertCEnum(format);
            return *this;
        }
        ImageCreateInfo& setExtent(const Extent3D& extent) {
            this->extent = extent.getStruct();
            return *this;
        }

        ImageCreateInfo& setMipLevels(uint32_t mipLevels) {
            this->mipLevels = mipLevels;
            return *this;
        }

        ImageCreateInfo& setArrayLayers(uint32_t arrayLayers) {
            this->arrayLayers = arrayLayers;
            return *this;
        }
        ImageCreateInfo& setSamples(Flags::SampleCount::Bits samples) {
            this->samples = Flags::convertCBit(samples);
            return *this;
        }
        ImageCreateInfo& setTiling(ImageTiling tiling) {
            this->tiling = convertCEnum(tiling);
            return *this;
        }
        ImageCreateInfo& setUsage(Flags::ImageUsage usage) {
            this->usage = usage;
            return *this;
        }
        ImageCreateInfo& setSharingMode(SharingMode sharingMode) {
            this->sharingMode = convertCEnum(sharingMode);
            return *this;
        }
        ImageCreateInfo& setQueueFamilyIndices(std::span<const uint32_t> queueFamilyIndices) {
            this->queueFamilyIndexCount = queueFamilyIndices.size();
            this->pQueueFamilyIndices = queueFamilyIndices.data();
            return *this;
        }
        ImageCreateInfo& setInitialLayout(ImageLayout initialLayout) {
            this->initialLayout = convertCEnum(initialLayout);
            return *this;
        }

        ImageType getImageType() const {
            return convertCEnum(this->imageType);
        }

        Format getFormat() const {
            return convertCEnum(this->format);
        }

        const Extent3D& getExtent() const {
            return Extent3D::underlyingCast(this->extent);
        }

        uint32_t getMipLevels() const { return this->mipLevels; }

        uint32_t getArrayLayers() const { return this->arrayLayers; }

        Flags::SampleCount::Bits getSamples() const { 
            return static_cast<Flags::SampleCount::Bits>(this->samples);
        }

        ImageTiling getTiling() const {
            return convertCEnum(this->tiling);
        }

        Flags::ImageUsage getUsage() const { return this->usage; }

        SharingMode getSharingMode() const {
            return convertCEnum(this->sharingMode);
        }

        uint32_t getQueueFamilyIndexCount() const { return this->queueFamilyIndexCount; }

        std::span<const uint32_t> getQueueFamilyIndices() const { 
            return std::span(this->pQueueFamilyIndices, this->queueFamilyIndexCount);
        }

        ImageLayout getInitialLayout() const {
            return convertCEnum(initialLayout);
        }
    };

    class ImageSubresourceRange : public StructBase<VkImageSubresourceRange, ImageSubresourceRange>
    {
        using Base = StructBase<VkImageSubresourceRange, ImageSubresourceRange>;
    public:
        using Base::Base;
        ImageSubresourceRange(Flags::ImageAspect aspectMask = Flags::ImageAspect::Bits::Color,
            uint32_t baseMipLevel = 0, uint32_t levelCount = 1,
            uint32_t baseArrayLayer = 0, uint32_t layerCount = 1) : Base() {
            this->aspectMask = aspectMask;
            this->baseMipLevel = baseMipLevel;
            this->levelCount = levelCount;
            this->baseArrayLayer = baseArrayLayer;
            this->layerCount = layerCount;
        }
        ImageSubresourceRange& setAspectMask(Flags::ImageAspect aspectMask) {
            this->aspectMask = aspectMask;
            return *this;
        }
        ImageSubresourceRange& setBaseMipLevel(uint32_t baseMipLevel) {
            this->baseMipLevel = baseMipLevel;
            return *this;
        }
        ImageSubresourceRange& setLevelCount(uint32_t levelCount) {
            this->levelCount = levelCount;
            return *this;
        }
        ImageSubresourceRange& setBaseArrayLayer(uint32_t baseArrayLayer) {
            this->baseArrayLayer = baseArrayLayer;
            return *this;
        }
        ImageSubresourceRange& setLayerCount(uint32_t layerCount) {
            this->layerCount = layerCount;
            return *this;
        }
    };

    class ImageViewCreateInfo : public StructBase<VkImageViewCreateInfo, ImageViewCreateInfo>
    {
        using Base = StructBase<VkImageViewCreateInfo, ImageViewCreateInfo>;
    public:
        using Base::Base;
        ImageViewCreateInfo(ImageRef image,
            ImageViewType viewType = ImageViewType::T2D,
            Format format = Format::R8G8B8A8Srgb,
            const ComponentMapping& components = ComponentMapping(),
            const ImageSubresourceRange& subresourceRange = ImageSubresourceRange()) : Base() {
            this->image = image.getHandle();
            this->viewType = convertCEnum(viewType);
            this->format = convertCEnum(format);
            this->components = components.getStruct();
            this->subresourceRange = subresourceRange.getStruct();
        }
        ImageViewCreateInfo& setImage(ImageRef image) {
            this->image = image.getHandle();
            return *this;
        }
        ImageViewCreateInfo& setViewType(ImageViewType viewType) {
            this->viewType = convertCEnum(viewType);
            return *this;
        }
        ImageViewCreateInfo& setFormat(Format format) {
            this->format = convertCEnum(format);
            return *this;
        }
        ImageViewCreateInfo& setComponents(const ComponentMapping& components) {
            this->components = components.getStruct();
            return *this;
        }
        ImageViewCreateInfo& setSubresourceRange(const ImageSubresourceRange& range) {
            this->subresourceRange = range.getStruct();
            return *this;
        }

        ComponentMapping& getComponents() {
            return *reinterpret_cast<ComponentMapping*>(&this->components);
        }

        ImageSubresourceRange& getSubresourceRange() {
            return *reinterpret_cast<ImageSubresourceRange*>(&this->subresourceRange);
        }
    };

    class ImageViewRef : public BaseComponent<VkImageView, ImageViewRef> {
        using Base = BaseComponent<VkImageView, ImageViewRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "Image::View";
    };

    class ImageView : public VerificatorComponent<VkImageView, ImageViewRef>
    {
        using Base = VerificatorComponent<VkImageView, ImageViewRef>;
    public:
        using Base::Base;        

        void create(const DeviceFunctionTable& functions, DeviceRef device,
            const ImageViewCreateInfo& createInfo);
        void destroy(const DeviceFunctionTable& functions, DeviceRef device);
    };

    class ImageMemoryBarrier : public StructBase<VkImageMemoryBarrier, ImageMemoryBarrier> {
        using Base = StructBase<VkImageMemoryBarrier, ImageMemoryBarrier>;
    public:
        using Base::Base;

        static inline const auto s_queueFamilyIgnored = VK_QUEUE_FAMILY_IGNORED;

        ImageMemoryBarrier(const ImageRef& image,
            ImageLayout oldLayout = ImageLayout::Undefined,
            ImageLayout newLayout = ImageLayout::Undefined,
            Flags::Access srcAccessMask = Flags::Access::Bits::None,
            Flags::Access dstAccessMask = Flags::Access::Bits::None,
            uint32_t srcQueueFamilyIndex = s_queueFamilyIgnored,
            uint32_t dstQueueFamilyIndex = s_queueFamilyIgnored,
            const ImageSubresourceRange& subresourceRange = ImageSubresourceRange()) : Base() {
            this->oldLayout = convertCEnum(oldLayout);
            this->newLayout = convertCEnum(newLayout);
            this->srcAccessMask = srcAccessMask;
            this->dstAccessMask = dstAccessMask;
            this->srcQueueFamilyIndex = srcQueueFamilyIndex;
            this->dstQueueFamilyIndex = dstQueueFamilyIndex;
            this->image = image;
            this->subresourceRange = subresourceRange.getStruct();
        }

        ImageMemoryBarrier& setOldLayout(ImageLayout oldLayout) {
            this->oldLayout = convertCEnum(oldLayout);
            return *this;
        }

        ImageMemoryBarrier& setNewLayout(ImageLayout newLayout) {
            this->newLayout = convertCEnum(newLayout);
            return *this;
        }

        ImageMemoryBarrier& setSrcAccessMask(Flags::Access srcAccessMask) {
            this->srcAccessMask = srcAccessMask;
            return *this;
        }

        ImageMemoryBarrier& setDstAccessMask(Flags::Access dstAccessMask) {
            this->dstAccessMask = dstAccessMask;
            return *this;
        }

        ImageMemoryBarrier& setSrcQueueFamilyIndex(uint32_t srcQueueFamilyIndex) {
            this->srcQueueFamilyIndex = srcQueueFamilyIndex;
            return *this;
        }

        ImageMemoryBarrier& setDstQueueFamilyIndex(uint32_t dstQueueFamilyIndex) {
            this->dstQueueFamilyIndex = dstQueueFamilyIndex;
            return *this;
        }

        ImageMemoryBarrier& setImage(const ImageRef& image) {
            this->image = image;
            return *this;
        }

        ImageMemoryBarrier& setSubresourceRange(const ImageSubresourceRange& subresourceRange) {
            this->subresourceRange = subresourceRange;
            return *this;
        }

        ImageSubresourceRange& getSubresourceRange() {
            return *reinterpret_cast<ImageSubresourceRange*>(&this->subresourceRange);
        }
    };

    class ImageSubresourceLayers : public StructBase<VkImageSubresourceLayers, ImageSubresourceLayers> {
        using Base = StructBase<VkImageSubresourceLayers, ImageSubresourceLayers>;
    public:
        using Base::Base;

        ImageSubresourceLayers(Flags::ImageAspect aspectMask = Flags::ImageAspect::Bits::Color,
            uint32_t mipLevel = 0, uint32_t baseArrayLayer = 0, uint32_t layerCount = 1) : Base() {
            this->aspectMask = aspectMask;
            this->mipLevel = mipLevel;
            this->baseArrayLayer = baseArrayLayer;
            this->layerCount = layerCount;
        }

        ImageSubresourceLayers& setAspectMask(Flags::ImageAspect aspectMask) {
            this->aspectMask = aspectMask;
            return *this;
        }
        ImageSubresourceLayers& setMipLevel(uint32_t mipLevel) {
            this->mipLevel = mipLevel;
            return *this;
        }
        ImageSubresourceLayers& setBaseArrayLayer(uint32_t baseArrayLayer) {
            this->baseArrayLayer = baseArrayLayer;
            return *this;
        }
        ImageSubresourceLayers& setLayerCount(uint32_t layerCount) {
            this->layerCount = layerCount;
            return *this;
        }
    };

    class ImageBlit : public StructBase<VkImageBlit, ImageBlit> {
        using Base = StructBase<VkImageBlit, ImageBlit>;
    public:
        using Base::Base;

        ImageBlit(const ImageSubresourceLayers& srcSubresource,
            std::span<const Offset3D, 2> srcOffsets,
            const ImageSubresourceLayers& dstSubresource,
            std::span<const Offset3D, 2> dstOffsets) : Base() {
            this->srcSubresource = srcSubresource;
            this->srcOffsets[0] = srcOffsets[0];
            this->srcOffsets[1] = srcOffsets[1];
            this->dstSubresource = dstSubresource;
            this->dstOffsets[0] = dstOffsets[0];
            this->dstOffsets[1] = dstOffsets[1];
        }

        ImageBlit(const ImageSubresourceLayers& srcSubresource,
            const Offset3D srcOffset1, const Offset3D srcOffset2,
            const ImageSubresourceLayers& dstSubresource,
            const Offset3D dstOffset1, const Offset3D dstOffset2) : Base() {
            this->srcSubresource = srcSubresource;
            this->srcOffsets[0] = srcOffset1;
            this->srcOffsets[1] = srcOffset2;
            this->dstSubresource = dstSubresource;
            this->dstOffsets[0] = dstOffset1;
            this->dstOffsets[1] = dstOffset2;
        }

        ImageBlit& setSrcOffsets(const Offset3D srcOffset1, const Offset3D srcOffset2) {
            this->srcOffsets[0] = srcOffset1.getStruct();
            this->srcOffsets[1] = srcOffset2.getStruct();
            return *this;
        }

        ImageBlit& setSrcOffsets(std::span<const Offset3D, 2> srcOffsets) {
            this->srcOffsets[0] = srcOffsets[0];
            this->srcOffsets[1] = srcOffsets[1];
            return *this;
        }

        ImageBlit& setDstOffsets(const Offset3D dstOffset1, const Offset3D dstOffset2) {
            this->srcOffsets[0] = dstOffset1.getStruct();
            this->srcOffsets[1] = dstOffset2.getStruct();
            return *this;
        }

        ImageBlit& setDstOffsets(std::span<const Offset3D, 2> dstOffsets) {
            this->dstOffsets[0] = dstOffsets[0];
            this->dstOffsets[1] = dstOffsets[1];
            return *this;
        }

        ImageBlit& setDstSubresource(const ImageSubresourceLayers& dstSubresource) {
            this->dstSubresource = dstSubresource.getStruct();
            return *this;
        }

        ImageBlit& setSrcSubresource(const ImageSubresourceLayers& srcSubresource) {
            this->srcSubresource = srcSubresource.getStruct();
            return *this;
        }

        ImageSubresourceLayers& getSrcSubresource() {
            return *reinterpret_cast<ImageSubresourceLayers*>(&this->srcSubresource);
        }

        ImageSubresourceLayers& getDstSubresource() {
            return *reinterpret_cast<ImageSubresourceLayers*>(&this->dstSubresource);
        }
    };

    class Image : public VerificatorComponent<VkImage, ImageRef>
    {
        friend class SwapChain;
        using Base = VerificatorComponent<VkImage, ImageRef>;
    public:
        using Base::Base;

        void create(const DeviceFunctionTable& functions, DeviceRef device,
            const ImageCreateInfo& createInfo);

        void destroy(const DeviceFunctionTable& functions, DeviceRef device);
    };

}

