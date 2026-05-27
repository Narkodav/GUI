#pragma once
#include "Graphics/FunctionTable.h"

#include "Graphics/Enums.h"

#ifdef _WIN32
#include "Graphics/PlatformManagement/TypeDefinitionsWin32.h"
#else
#include "Graphics/PlatformManagement/TypeDefinitionsXlib.h"
#endif

#include "Graphics/FunctionPointers.h"

namespace Graphics {

    template <InstanceFunction function>
    struct InstanceFunctionTraits {};

    class InstanceRef;

    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateDevice> {
        using Type = PFN_createDevice;
        static constexpr const char* name = "vkCreateDevice";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::DestroyInstance> {
        using Type = PFN_destroyInstance;
        static constexpr const char* name = "vkDestroyInstance";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::EnumeratePhysicalDevices> {
        using Type = PFN_enumeratePhysicalDevices;
        static constexpr const char* name = "vkEnumeratePhysicalDevices";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceFeatures> {
        using Type = PFN_getPhysicalDeviceFeatures;
        static constexpr const char* name = "vkGetPhysicalDeviceFeatures";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceFormatProperties> {
        using Type = PFN_getPhysicalDeviceFormatProperties;
        static constexpr const char* name = "vkGetPhysicalDeviceFormatProperties";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceImageFormatProperties> {
        using Type = PFN_vkGetPhysicalDeviceImageFormatProperties;
        static constexpr const char* name = "vkGetPhysicalDeviceImageFormatProperties";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceProperties> {
        using Type = PFN_getPhysicalDeviceProperties;
        static constexpr const char* name = "vkGetPhysicalDeviceProperties";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceQueueFamilyProperties> {
        using Type = PFN_getPhysicalDeviceQueueFamilyProperties;
        static constexpr const char* name = "vkGetPhysicalDeviceQueueFamilyProperties";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceMemoryProperties> {
        using Type = PFN_getPhysicalDeviceMemoryProperties;
        static constexpr const char* name = "vkGetPhysicalDeviceMemoryProperties";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceSparseImageFormatProperties> {
        using Type = PFN_vkGetPhysicalDeviceSparseImageFormatProperties;
        static constexpr const char* name = "vkGetPhysicalDeviceSparseImageFormatProperties";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetDeviceProcAddr> {
        using Type = PFN_vkGetDeviceProcAddr;
        static constexpr const char* name = "vkGetDeviceProcAddr";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::EnumerateDeviceExtensionProperties> {
        using Type = PFN_enumerateDeviceExtensionProperties;
        static constexpr const char* name = "vkEnumerateDeviceExtensionProperties";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::EnumerateDeviceLayerProperties> {
        using Type = PFN_enumerateDeviceLayerProperties;
        static constexpr const char* name = "vkEnumerateDeviceLayerProperties";
    };

    // VK_KHR_surface extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::DestroySurfaceKHR> {
        using Type = PFN_destroySurfaceKHR;
        static constexpr const char* name = "vkDestroySurfaceKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceSurfaceSupportKHR> {
        using Type = PFN_getPhysicalDeviceSurfaceSupportKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceSurfaceSupportKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceSurfaceCapabilitiesKHR> {
        using Type = PFN_getPhysicalDeviceSurfaceCapabilitiesKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceSurfaceCapabilitiesKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceSurfaceFormatsKHR> {
        using Type = PFN_getPhysicalDeviceSurfaceFormatsKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceSurfaceFormatsKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceSurfacePresentModesKHR> {
        using Type = PFN_getPhysicalDeviceSurfacePresentModesKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceSurfacePresentModesKHR";
    };

    // VK_EXT_debug_utils extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateDebugUtilsMessengerEXT> {
        using Type = PFN_createDebugUtilsMessengerEXT;
        static constexpr const char* name = "vkCreateDebugUtilsMessengerEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::DestroyDebugUtilsMessengerEXT> {
        using Type = PFN_destroyDebugUtilsMessengerEXT;
        static constexpr const char* name = "vkDestroyDebugUtilsMessengerEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::SubmitDebugUtilsMessageEXT> {
        using Type = PFN_vkSubmitDebugUtilsMessageEXT;
        static constexpr const char* name = "vkSubmitDebugUtilsMessageEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::SetDebugUtilsObjectNameEXT> {
        using Type = PFN_setDebugUtilsObjectNameEXT;
        static constexpr const char* name = "vkSetDebugUtilsObjectNameEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::SetDebugUtilsObjectTagEXT> {
        using Type = PFN_vkSetDebugUtilsObjectTagEXT;
        static constexpr const char* name = "vkSetDebugUtilsObjectTagEXT";
    };

    // VK_EXT_debug_report extension (legacy)
    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateDebugReportCallbackEXT> {
        using Type = PFN_createDebugReportCallbackEXT;
        static constexpr const char* name = "vkCreateDebugReportCallbackEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::DestroyDebugReportCallbackEXT> {
        using Type = PFN_destroyDebugReportCallbackEXT;
        static constexpr const char* name = "vkDestroyDebugReportCallbackEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::DebugReportMessageEXT> {
        using Type = PFN_vkDebugReportMessageEXT;
        static constexpr const char* name = "vkDebugReportMessageEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceFeatures2> {
        using Type = PFN_vkGetPhysicalDeviceFeatures2;
        static constexpr const char* name = "vkGetPhysicalDeviceFeatures2";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceProperties2> {
        using Type = PFN_vkGetPhysicalDeviceProperties2;
        static constexpr const char* name = "vkGetPhysicalDeviceProperties2";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceFormatProperties2> {
        using Type = PFN_vkGetPhysicalDeviceFormatProperties2;
        static constexpr const char* name = "vkGetPhysicalDeviceFormatProperties2";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceImageFormatProperties2> {
        using Type = PFN_vkGetPhysicalDeviceImageFormatProperties2;
        static constexpr const char* name = "vkGetPhysicalDeviceImageFormatProperties2";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceQueueFamilyProperties2> {
        using Type = PFN_vkGetPhysicalDeviceQueueFamilyProperties2;
        static constexpr const char* name = "vkGetPhysicalDeviceQueueFamilyProperties2";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceMemoryProperties2> {
        using Type = PFN_vkGetPhysicalDeviceMemoryProperties2;
        static constexpr const char* name = "vkGetPhysicalDeviceMemoryProperties2";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceSparseImageFormatProperties2> {
        using Type = PFN_vkGetPhysicalDeviceSparseImageFormatProperties2;
        static constexpr const char* name = "vkGetPhysicalDeviceSparseImageFormatProperties2";
    };

    // VK_KHR_external_fence_capabilities extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceExternalFenceProperties> {
        using Type = PFN_getPhysicalDeviceExternalFenceProperties;
        static constexpr const char* name = "vkGetPhysicalDeviceExternalFenceProperties";
    };

    // VK_KHR_external_semaphore_capabilities extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceExternalSemaphoreProperties> {
        using Type = PFN_getPhysicalDeviceExternalSemaphoreProperties;
        static constexpr const char* name = "vkGetPhysicalDeviceExternalSemaphoreProperties";
    };

    // VK_KHR_external_memory_capabilities extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceExternalBufferProperties> {
        using Type = PFN_getPhysicalDeviceExternalBufferProperties;
        static constexpr const char* name = "vkGetPhysicalDeviceExternalBufferProperties";
    };

    // VK_KHR_get_physical_device_properties2 extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceFeatures2KHR> {
        using Type = PFN_vkGetPhysicalDeviceFeatures2KHR;
        static constexpr const char* name = "vkGetPhysicalDeviceFeatures2KHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceProperties2KHR> {
        using Type = PFN_vkGetPhysicalDeviceProperties2KHR;
        static constexpr const char* name = "vkGetPhysicalDeviceProperties2KHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceFormatProperties2KHR> {
        using Type = PFN_vkGetPhysicalDeviceFormatProperties2KHR;
        static constexpr const char* name = "vkGetPhysicalDeviceFormatProperties2KHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceImageFormatProperties2KHR> {
        using Type = PFN_vkGetPhysicalDeviceImageFormatProperties2KHR;
        static constexpr const char* name = "vkGetPhysicalDeviceImageFormatProperties2KHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceQueueFamilyProperties2KHR> {
        using Type = PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR;
        static constexpr const char* name = "vkGetPhysicalDeviceQueueFamilyProperties2KHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceMemoryProperties2KHR> {
        using Type = PFN_vkGetPhysicalDeviceMemoryProperties2KHR;
        static constexpr const char* name = "vkGetPhysicalDeviceMemoryProperties2KHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceSparseImageFormatProperties2KHR> {
        using Type = PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR;
        static constexpr const char* name = "vkGetPhysicalDeviceSparseImageFormatProperties2KHR";
    };

    // VK_KHR_external_fence_capabilities extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceExternalFencePropertiesKHR> {
        using Type = PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceExternalFencePropertiesKHR";
    };

    // VK_KHR_external_semaphore_capabilities extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceExternalSemaphorePropertiesKHR> {
        using Type = PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR";
    };

    // VK_KHR_external_memory_capabilities extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceExternalBufferPropertiesKHR> {
        using Type = PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceExternalBufferPropertiesKHR";
    };

    // VK_KHR_get_surface_capabilities2 extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceSurfaceCapabilities2KHR> {
        using Type = PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR;
        static constexpr const char* name = "vkGetPhysicalDeviceSurfaceCapabilities2KHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceSurfaceFormats2KHR> {
        using Type = PFN_vkGetPhysicalDeviceSurfaceFormats2KHR;
        static constexpr const char* name = "vkGetPhysicalDeviceSurfaceFormats2KHR";
    };

    // VK_KHR_performance_query extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR> {
        using Type = PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR;
        static constexpr const char* name = "vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR> {
        using Type = PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR";
    };

    // VK_EXT_headless_surface extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateHeadlessSurfaceEXT> {
        using Type = PFN_vkCreateHeadlessSurfaceEXT;
        static constexpr const char* name = "vkCreateHeadlessSurfaceEXT";
    };

    // VK_KHR_device_group_creation extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::EnumeratePhysicalDeviceGroupsKHR> {
        using Type = PFN_vkEnumeratePhysicalDeviceGroupsKHR;
        static constexpr const char* name = "vkEnumeratePhysicalDeviceGroupsKHR";
    };

    // VK_KHR_fragment_shading_rate extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceFragmentShadingRatesKHR> {
        using Type = PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceFragmentShadingRatesKHR";
    };

    // VK_EXT_tooling_info extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceToolPropertiesEXT> {
        using Type = PFN_vkGetPhysicalDeviceToolPropertiesEXT;
        static constexpr const char* name = "vkGetPhysicalDeviceToolPropertiesEXT";
    };

    // VK_EXT_acquire_drm_display extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::AcquireDrmDisplayEXT> {
        using Type = PFN_vkAcquireDrmDisplayEXT;
        static constexpr const char* name = "vkAcquireDrmDisplayEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetDrmDisplayEXT> {
        using Type = PFN_vkGetDrmDisplayEXT;
        static constexpr const char* name = "vkGetDrmDisplayEXT";
    };

    // VK_NV_cooperative_matrix extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceCooperativeMatrixPropertiesNV> {
        using Type = PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV;
        static constexpr const char* name = "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV";
    };

    // VK_NV_optical_flow extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceOpticalFlowImageFormatsNV> {
        using Type = PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV;
        static constexpr const char* name = "vkGetPhysicalDeviceOpticalFlowImageFormatsNV";
    };

    // VK_KHR_display extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceDisplayPropertiesKHR> {
        using Type = PFN_vkGetPhysicalDeviceDisplayPropertiesKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceDisplayPropertiesKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceDisplayPlanePropertiesKHR> {
        using Type = PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceDisplayPlanePropertiesKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetDisplayPlaneSupportedDisplaysKHR> {
        using Type = PFN_vkGetDisplayPlaneSupportedDisplaysKHR;
        static constexpr const char* name = "vkGetDisplayPlaneSupportedDisplaysKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetDisplayModePropertiesKHR> {
        using Type = PFN_vkGetDisplayModePropertiesKHR;
        static constexpr const char* name = "vkGetDisplayModePropertiesKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateDisplayModeKHR> {
        using Type = PFN_vkCreateDisplayModeKHR;
        static constexpr const char* name = "vkCreateDisplayModeKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetDisplayPlaneCapabilitiesKHR> {
        using Type = PFN_vkGetDisplayPlaneCapabilitiesKHR;
        static constexpr const char* name = "vkGetDisplayPlaneCapabilitiesKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateDisplayPlaneSurfaceKHR> {
        using Type = PFN_vkCreateDisplayPlaneSurfaceKHR;
        static constexpr const char* name = "vkCreateDisplayPlaneSurfaceKHR";
    };

    // VK_KHR_swapchain extension
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDevicePresentRectanglesKHR> {
        using Type = PFN_vkGetPhysicalDevicePresentRectanglesKHR;
        static constexpr const char* name = "vkGetPhysicalDevicePresentRectanglesKHR";
    };

    // Missing VK_EXT_debug_utils functions
    template <>
    struct InstanceFunctionTraits<InstanceFunction::CmdBeginDebugUtilsLabelEXT> {
        using Type = PFN_vkCmdBeginDebugUtilsLabelEXT;
        static constexpr const char* name = "vkCmdBeginDebugUtilsLabelEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::CmdEndDebugUtilsLabelEXT> {
        using Type = PFN_vkCmdEndDebugUtilsLabelEXT;
        static constexpr const char* name = "vkCmdEndDebugUtilsLabelEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::CmdInsertDebugUtilsLabelEXT> {
        using Type = PFN_vkCmdInsertDebugUtilsLabelEXT;
        static constexpr const char* name = "vkCmdInsertDebugUtilsLabelEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::QueueBeginDebugUtilsLabelEXT> {
        using Type = PFN_vkQueueBeginDebugUtilsLabelEXT;
        static constexpr const char* name = "vkQueueBeginDebugUtilsLabelEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::QueueEndDebugUtilsLabelEXT> {
        using Type = PFN_vkQueueEndDebugUtilsLabelEXT;
        static constexpr const char* name = "vkQueueEndDebugUtilsLabelEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::QueueInsertDebugUtilsLabelEXT> {
        using Type = PFN_vkQueueInsertDebugUtilsLabelEXT;
        static constexpr const char* name = "vkQueueInsertDebugUtilsLabelEXT";
    };

#ifdef _WIN32
    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateWin32SurfaceKHR> {
        using Type = Win32::PFN_CreateSurface;
        static constexpr const char* name = "vkCreateWin32SurfaceKHR";
    };
#else
    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateXlibSurfaceKHR> {
        using Type = Xlib::PFN_CreateSurface;
        static constexpr const char* name = "vkCreateXlibSurfaceKHR";
    };
#endif

    // Missing VK_NV_copy_memory_indirect function
    /*
    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceGeneratedCommandsPropertiesNV> {
        using Type = PFN_vkGetPhysicalDeviceGeneratedCommandsPropertiesNV;
        static constexpr const char* name = "vkGetPhysicalDeviceGeneratedCommandsPropertiesNV";
    };
    */

    // Missing surface creation functions
    /*
    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateMetalSurfaceEXT> {
        using Type = PFN_vkCreateMetalSurfaceEXT;
        static constexpr const char* name = "vkCreateMetalSurfaceEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateWaylandSurfaceKHR> {
        using Type = PFN_vkCreateWaylandSurfaceKHR;
        static constexpr const char* name = "vkCreateWaylandSurfaceKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceWaylandPresentationSupportKHR> {
        using Type = PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceWaylandPresentationSupportKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceWin32PresentationSupportKHR> {
        using Type = PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceWin32PresentationSupportKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateXlibSurfaceKHR> {
        using Type = PFN_vkCreateXlibSurfaceKHR;
        static constexpr const char* name = "vkCreateXlibSurfaceKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceXlibPresentationSupportKHR> {
        using Type = PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceXlibPresentationSupportKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateXcbSurfaceKHR> {
        using Type = PFN_vkCreateXcbSurfaceKHR;
        static constexpr const char* name = "vkCreateXcbSurfaceKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceXcbPresentationSupportKHR> {
        using Type = PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR;
        static constexpr const char* name = "vkGetPhysicalDeviceXcbPresentationSupportKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateAndroidSurfaceKHR> {
        using Type = PFN_vkCreateAndroidSurfaceKHR;
        static constexpr const char* name = "vkCreateAndroidSurfaceKHR";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateIOSSurfaceMVK> {
        using Type = PFN_vkCreateIOSSurfaceMVK;
        static constexpr const char* name = "vkCreateIOSSurfaceMVK";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateMacOSSurfaceMVK> {
        using Type = PFN_vkCreateMacOSSurfaceMVK;
        static constexpr const char* name = "vkCreateMacOSSurfaceMVK";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateStreamDescriptorSurfaceGGP> {
        using Type = PFN_vkCreateStreamDescriptorSurfaceGGP;
        static constexpr const char* name = "vkCreateStreamDescriptorSurfaceGGP";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::CreateViSurfaceNN> {
        using Type = PFN_vkCreateViSurfaceNN;
        static constexpr const char* name = "vkCreateViSurfaceNN";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetPhysicalDeviceSurfacePresentModes2EXT> {
        using Type = PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT;
        static constexpr const char* name = "vkGetPhysicalDeviceSurfacePresentModes2EXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::AcquireXlibDisplayEXT> {
        using Type = PFN_vkAcquireXlibDisplayEXT;
        static constexpr const char* name = "vkAcquireXlibDisplayEXT";
    };

    template <>
    struct InstanceFunctionTraits<InstanceFunction::GetRandROutputDisplayEXT> {
        using Type = PFN_vkGetRandROutputDisplayEXT;
        static constexpr const char* name = "vkGetRandROutputDisplayEXT";
    };
    */
    using InstanceFunctionTable = FunctionTable<InstanceFunction, InstanceFunctionTraits, VkInstance, PFN_vkGetInstanceProcAddr>;
}