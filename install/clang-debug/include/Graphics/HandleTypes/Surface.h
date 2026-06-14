#pragma once
#include "Graphics/Common.h"
#include "Graphics/Structs.h"
#include "Graphics/InstanceFunctionTable.h"
#include "PlatformKit/Window.h"

namespace Graphics
{
    class InstanceRef;

    class SurfaceRef : public BaseComponent<VkSurfaceKHR, SurfaceRef>
    {
        using Base = BaseComponent<VkSurfaceKHR, SurfaceRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "Surface";
    };

    class SurfaceFormat : public StructBase<VkSurfaceFormatKHR, SurfaceFormat> {
        using Base = StructBase<VkSurfaceFormatKHR, SurfaceFormat>;
    public:
        using Base::Base;

        ColorSpace getColorSpace() const {
            return convertCEnum(this->colorSpace);
        }

        Format getFormat() const {
            return convertCEnum(this->format);
        }
    };

    class SurfaceCapabilities : public StructBase<VkSurfaceCapabilitiesKHR, SurfaceCapabilities> {
        using Base = StructBase<VkSurfaceCapabilitiesKHR, SurfaceCapabilities>;
    public:
        using Base::Base;

        Extent2D getMinImageExtent() const {
            return this->minImageExtent;
        }

        Extent2D getMaxImageExtent() const {
            return this->maxImageExtent;
        }

        Extent2D getCurrentExtent() const {
            return this->currentExtent;
        }

        Flags::SurfaceTransform getCurrentTransform() const {
            return Flags::SurfaceTransform(this->currentTransform);
        }

        uint32_t getMaxImageArrayLayers() const {
            return this->maxImageArrayLayers;
        }

        uint32_t getMaxImageCount() const {
            return this->maxImageCount;
        }

        uint32_t getMinImageCount() const {
            return this->minImageCount;
        }
    };

    class Surface : public VerificatorComponent<VkSurfaceKHR, SurfaceRef>
    {
        using Base = VerificatorComponent<VkSurfaceKHR, SurfaceRef>;
    public:
        using Base::Base;

        void create(const InstanceFunctionTable& functions, const InstanceRef& instance, const PlatformKit::Window& window);
        void destroy(const InstanceFunctionTable& functions, const InstanceRef& instance);
    };
}

