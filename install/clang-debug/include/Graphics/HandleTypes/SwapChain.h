#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Surface.h"
#include "Image.h"

namespace Graphics {

    class ImageRef;
    class SemaphoreRef;
    class FenceRef;
    class DeviceRef;
    class SwapChain;
    class SwapChainRef;

    class SwapChainRef : public BaseComponent<VkSwapchainKHR, SwapChainRef> 
    {
        using Base = BaseComponent<VkSwapchainKHR, SwapChainRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "SwapChain";

        std::vector<ImageRef> getImages(const DeviceFunctionTable& functions, DeviceRef device) const;
    };

    class SwapChainCreateInfo : public StructBase<VkSwapchainCreateInfoKHR, SwapChainCreateInfo>
    {
        using Base = StructBase<VkSwapchainCreateInfoKHR, SwapChainCreateInfo>;
    public:
        using Base::Base;
        SwapChainCreateInfo(SurfaceRef surface,
            const Extent2D& imageExtent = { 1, 1 },
            uint32_t minImageCount = 2,
            uint32_t imageArrayLayers = 1,
            Format imageFormat = Format::B8G8R8A8Srgb,
            ColorSpace imageColorSpace = ColorSpace::SrgbNonlinear,
            Flags::ImageUsage imageUsage = Flags::ImageUsage::Bits::ColorAttachment,
            SharingMode imageSharingMode = SharingMode::Exclusive,
            std::span<const uint32_t> queueFamilyIndices = {},
            Flags::SurfaceTransform::Bits preTransform = Flags::SurfaceTransform::Bits::Identity,
            Flags::CompositeAlpha::Bits compositeAlpha = Flags::CompositeAlpha::Bits::Opaque,
            PresentMode presentMode = PresentMode::Fifo,
            bool clipped = true,
            SwapChainRef oldSwapChain = SwapChainRef()) : Base()
        {
            this->surface = surface.getHandle();
            this->minImageCount = minImageCount;
            this->imageFormat = convertCEnum(imageFormat);
            this->imageColorSpace = convertCEnum(imageColorSpace);
            this->imageExtent = imageExtent.getStruct();
            this->imageArrayLayers = imageArrayLayers;
            this->imageUsage = imageUsage;
            this->imageSharingMode = convertCEnum(imageSharingMode);
            this->queueFamilyIndexCount = queueFamilyIndices.size();
            this->pQueueFamilyIndices = queueFamilyIndices.data();
            this->preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(preTransform);
            this->compositeAlpha = static_cast<VkCompositeAlphaFlagBitsKHR>(compositeAlpha);
            this->presentMode = convertCEnum(presentMode);
            this->clipped = clipped;
            this->oldSwapchain = oldSwapChain.getHandle();
        }

        SwapChainCreateInfo(SurfaceRef surface,
            const SurfaceFormat& surfaceFormat,
            const Extent2D& imageExtent = { 1, 1 },
            uint32_t minImageCount = 2,
            uint32_t imageArrayLayers = 1,
            Flags::ImageUsage imageUsage = Flags::ImageUsage::Bits::ColorAttachment,
            SharingMode imageSharingMode = SharingMode::Exclusive,
            const std::vector<uint32_t>& queueFamilyIndices = {},
            Flags::SurfaceTransform::Bits preTransform = Flags::SurfaceTransform::Bits::Identity,
            Flags::CompositeAlpha::Bits compositeAlpha = Flags::CompositeAlpha::Bits::Opaque,
            PresentMode presentMode = PresentMode::Fifo,
            bool clipped = true,
            SwapChainRef oldSwapChain = SwapChainRef()) : Base()
        {
            this->surface = surface.getHandle();
            this->minImageCount = minImageCount;
            this->imageFormat = convertCEnum(surfaceFormat.getFormat());
            this->imageColorSpace = convertCEnum(surfaceFormat.getColorSpace());
            this->imageExtent = imageExtent.getStruct();
            this->imageArrayLayers = imageArrayLayers;
            this->imageUsage = imageUsage;
            this->imageSharingMode = convertCEnum(imageSharingMode);
            this->queueFamilyIndexCount = queueFamilyIndices.size();
            this->pQueueFamilyIndices = queueFamilyIndices.data();
            this->preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(preTransform);
            this->compositeAlpha = static_cast<VkCompositeAlphaFlagBitsKHR>(compositeAlpha);
            this->presentMode = convertCEnum(presentMode);
            this->clipped = clipped;
            this->oldSwapchain = oldSwapChain.getHandle();
        }

        SwapChainCreateInfo& setSurface(SurfaceRef surface) {
            this->surface = surface.getHandle();
            return *this;
        }
        SwapChainCreateInfo& setMinImageCount(uint32_t minImageCount) {
            this->minImageCount = minImageCount;
            return *this;
        }
        SwapChainCreateInfo& setImageFormat(const SurfaceFormat& imageFormat) {
            this->imageFormat = convertCEnum(imageFormat.getFormat());
            this->imageColorSpace = convertCEnum(imageFormat.getColorSpace());
            return *this;
        }
        SwapChainCreateInfo& setImageFormat(Format imageFormat) {
            this->imageFormat = convertCEnum(imageFormat);
            return *this;
        }
        SwapChainCreateInfo& setImageColorSpace(ColorSpace imageColorSpace) {
            this->imageColorSpace = convertCEnum(imageColorSpace);
            return *this;
        }
        SwapChainCreateInfo& setImageExtent(const Extent2D& imageExtent) {
            this->imageExtent = imageExtent.getStruct();
            return *this;
        }
        SwapChainCreateInfo& setImageUsage(Flags::ImageUsage imageUsage) {
            this->imageUsage = imageUsage;
            return *this;
        }
        SwapChainCreateInfo& setImageSharingMode(SharingMode imageSharingMode) {
            this->imageSharingMode = convertCEnum(imageSharingMode);
            return *this;
        }
        SwapChainCreateInfo& setQueueFamilyIndices(std::span<const uint32_t> queueFamilyIndices) {
            this->queueFamilyIndexCount = queueFamilyIndices.size();
            this->pQueueFamilyIndices = queueFamilyIndices.data();
            return *this;
        }
        SwapChainCreateInfo& setPreTransform(Flags::SurfaceTransform::Bits preTransform) {
            this->preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(preTransform);
            return *this;
        }
        SwapChainCreateInfo& setCompositeAlpha(Flags::CompositeAlpha::Bits compositeAlpha) {
            this->compositeAlpha = static_cast<VkCompositeAlphaFlagBitsKHR>(compositeAlpha);
            return *this;
        }
        SwapChainCreateInfo& setPresentMode(PresentMode presentMode) {
            this->presentMode = convertCEnum(presentMode);
            return *this;
        }
        SwapChainCreateInfo& setClipped(bool clipped) {
            this->clipped = clipped;
            return *this;
        }
        SwapChainCreateInfo& setOldSwapChain(SwapChainRef oldSwapChain) {
            this->oldSwapchain = oldSwapChain.getHandle();
            return *this;
        }
        SwapChainCreateInfo& setImageArrayLayers(uint32_t imageArrayLayers) {
            this->imageArrayLayers = imageArrayLayers;
            return *this;
        }

        Format getImageFormat() const {
            return convertCEnum(this->imageFormat);
        };
        ColorSpace getImageColorSpace() const {
            return convertCEnum(this->imageColorSpace);
        };
        Extent2D getImageExtent() const {
            return Extent2D(this->imageExtent);
        };
        Flags::ImageUsage getImageUsage() const {
            return this->imageUsage;
        };
        SharingMode getImageSharingMode() const {
            return convertCEnum(this->imageSharingMode);
        };
        Flags::SurfaceTransform::Bits getPreTransform() const {
            return static_cast<Flags::SurfaceTransform::Bits>(this->preTransform);
        };
        Flags::CompositeAlpha::Bits getCompositeAlpha() const {
            return static_cast<Flags::CompositeAlpha::Bits>(this->compositeAlpha);
        };
        PresentMode getPresentMode() const {
            return convertCEnum(this->presentMode);
        };
        bool isClipped() const {
            return this->clipped;
        };
        SwapChainRef getOldSwapChain() const {
            return SwapChainRef(this->oldSwapchain);
        };
        SurfaceRef getSurface() const {
            return SurfaceRef(this->surface);
        };
        std::span<const uint32_t> getQueueFamilyIndices() const {
            return std::span<const uint32_t>(this->pQueueFamilyIndices, this->queueFamilyIndexCount);
        };
        uint32_t getImageArrayLayers() const {
            return this->imageArrayLayers;
        }
    };

	class SwapChain : public VerificatorComponent<VkSwapchainKHR, SwapChainRef>
	{
		using Base = VerificatorComponent<VkSwapchainKHR, SwapChainRef>;
	public:
		using Base::Base;

        void create(const DeviceFunctionTable& functions, DeviceRef device,
            const SwapChainCreateInfo& createInfo);
        void destroy(const DeviceFunctionTable& functions, DeviceRef device);

		//implicitly sets the old swap chain parameter in create info
        void recreate(const DeviceFunctionTable& functions, DeviceRef device,
            SwapChainCreateInfo& createInfo);

        Result acquireNextImage(const DeviceFunctionTable& functions, DeviceRef device,
            SemaphoreRef semaphore, FenceRef fence, uint32_t& imageIndex,
            uint32_t timeout = std::numeric_limits<uint32_t>::max());

        Result acquireNextImage(const DeviceFunctionTable& functions, DeviceRef device,
            SemaphoreRef semaphore, uint32_t& imageIndex, uint32_t timeout = std::numeric_limits<uint32_t>::max());

        Result acquireNextImage(const DeviceFunctionTable& functions, DeviceRef device,
            FenceRef fence, uint32_t& imageIndex, uint32_t timeout = std::numeric_limits<uint32_t>::max());
	};
}

