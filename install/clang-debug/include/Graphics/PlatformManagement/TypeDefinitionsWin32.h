#pragma once
#include "PlatformKit/WindowWin32.h"
#include <vulkan/vulkan.h>

#include "Graphics/Enums.h"
#include "Graphics/Flags.h"

namespace Graphics {
    class InstanceRef;
    class AllocationCallbacks;
    class Surface;
    class PhysicalDevice;
}

namespace Graphics::Win32 {

    struct SurfaceCreateInfo {
        StructureType                   sType;
        const void* pNext;
        Graphics::Flags::SurfaceCreate  flags;
        PlatformKit::Win32::InstanceHandle hinstance;
        PlatformKit::Win32::WindowHandle   hwnd;
    };
    
    using PFN_CreateSurface = Result(VKAPI_PTR*)(InstanceRef instance, const SurfaceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Surface* pSurface);
    using PFN_GetPhysicalDevicePresentationSupport = Bool32(VKAPI_PTR*)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex);
}