#pragma once
#include "Graphics/Common.h"
#include "Graphics/Flags.h"

namespace Graphics {

    // Forward declarations
    class Instance;
    class Device;
    class InstanceRef;
    class DeviceRef;
    class PhysicalDevice;
    class DeviceCreateInfo;
    class AllocationCallbacks;
    class PhysicalDeviceFeatures;
    class FormatProperties;
    class ImageFormatProperties;
    class PhysicalDeviceProperties;
    class QueueFamilyProperties;
    class PhysicalDeviceMemoryProperties;
    class SparseImageFormatProperties;
    class ExtensionProperties;
    class LayerProperties;
    class SurfaceRef;
    class SurfaceCapabilities;
    class SurfaceFormat;

    namespace DebugUtils {
        class Messenger;
        class MessengerRef;
        class MessengerCreateInfo;
        class ObjectNameInfo;
        //class ObjectTagInfo;
    }

    class DebugReportCallbackRef;
    class DebugReportCallbackCreateInfo;
    class PhysicalDeviceFeatures2;
    class PhysicalDeviceProperties2;
    class FormatProperties2;
    class ImageFormatProperties2;
    class QueueFamilyProperties2;
    class PhysicalDeviceMemoryProperties2;
    class SparseImageFormatProperties2;
    class PhysicalDeviceExternalFenceInfo;
    class ExternalFenceProperties;
    class PhysicalDeviceExternalSemaphoreInfo;
    class ExternalSemaphoreProperties;
    class PhysicalDeviceExternalBufferInfo;
    class ExternalBufferProperties;
    class SurfaceCapabilities2;
    class SurfaceFormat2;
    class PhysicalDeviceQueueFamilyPerformanceQueryCreateInfo;
    class PerformanceCounterKHR;
    class PerformanceCounterDescriptionKHR;
    class HeadlessSurfaceCreateInfo;
    class PhysicalDeviceGroupProperties;
    class PhysicalDeviceFragmentShadingRateKHR;
    class PhysicalDeviceToolProperties;
    class DisplayKHR;
    class DisplayPropertiesKHR;
    class DisplayPlanePropertiesKHR;
    class DisplayModePropertiesKHR;
    class DisplayModeCreateInfoKHR;
    class DisplayModeKHR;
    class DisplayPlaneCapabilitiesKHR;
    class DisplaySurfaceCreateInfoKHR;
    class Rect2D;
    class PhysicalDeviceCooperativeMatrixPropertiesNV;
    class OpticalFlowImageFormatInfoNV;
    class OpticalFlowImageFormatPropertiesNV;
    class DebugUtilsLabelEXT;
    class CommandBuffer;
    class Queue;

    using PFN_allocationFunction = void* (GRAPHICS_API_PTR*)(
        void* pUserData,
        size_t size,
        size_t alignment,
        SystemAllocationScope allocationScope);

    using PFN_freeFunction = void (GRAPHICS_API_PTR*)(
        void* pUserData,
        void* pMemory);

    using PFN_internalAllocationNotification = void (GRAPHICS_API_PTR*)(
        void* pUserData,
        size_t                                      size,
        InternalAllocationType                    allocationType,
        SystemAllocationScope                     allocationScope);

    using PFN_internalFreeNotification = void (GRAPHICS_API_PTR*)(
        void* pUserData,
        size_t                                      size,
        InternalAllocationType                    allocationType,
        SystemAllocationScope                     allocationScope);

    using PFN_reallocationFunction = void* (GRAPHICS_API_PTR*)(
        void* pUserData,
        void* pOriginal,
        size_t                                      size,
        size_t                                      alignment,
        SystemAllocationScope                     allocationScope);

    namespace DebugUtils {
        class CallbackData;
        using PFN_callback = Bool32(GRAPHICS_API_PTR*)(
            Flags::DebugMessageSeverity::Bits severity,
            Flags::DebugMessageType    type,
            const CallbackData* pCallbackData,
            void* pUserData
            );
    }
        

    // Instance function pointer types
    using PFN_destroyInstance = Result(VKAPI_PTR*)(InstanceRef instance, const AllocationCallbacks* pAllocator);
    using PFN_createDevice = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, const DeviceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Device* pDevice);
    using PFN_enumeratePhysicalDevices = Result(VKAPI_PTR*)(InstanceRef instance, uint32_t* pPhysicalDeviceCount, PhysicalDevice* pPhysicalDevices);
    using PFN_getPhysicalDeviceFeatures = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, PhysicalDeviceFeatures* pFeatures);
    using PFN_getPhysicalDeviceFormatProperties = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, Format format, FormatProperties* pFormatProperties);
    //using PFN_getPhysicalDeviceImageFormatProperties = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, Format format, ImageType type, ImageTiling tiling, Flags::ImageUsage usage, Flags::ImageCreate flags, ImageFormatProperties* pImageFormatProperties);
    using PFN_getPhysicalDeviceProperties = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, PhysicalDeviceProperties* pProperties);
    using PFN_getPhysicalDeviceQueueFamilyProperties = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, QueueFamilyProperties* pQueueFamilyProperties);
    using PFN_getPhysicalDeviceMemoryProperties = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, PhysicalDeviceMemoryProperties* pMemoryProperties);
    //using PFN_getPhysicalDeviceSparseImageFormatProperties = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, Format format, ImageType type, Flags::SampleCount::Bits samples, Flags::ImageUsage usage, ImageTiling tiling, uint32_t* pPropertyCount, SparseImageFormatProperties* pProperties);
    using PFN_getDeviceProcAddr = PFN_vkVoidFunction(VKAPI_PTR*)(DeviceRef device, const char* pName);
    using PFN_enumerateDeviceExtensionProperties = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties);
    using PFN_enumerateDeviceLayerProperties = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, LayerProperties* pProperties);
    using PFN_destroySurfaceKHR = void(VKAPI_PTR*)(InstanceRef instance, SurfaceRef surface, const AllocationCallbacks* pAllocator);
    using PFN_getPhysicalDeviceSurfaceSupportKHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, SurfaceRef surface, Bool32* pSupported);
    using PFN_getPhysicalDeviceSurfaceCapabilitiesKHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, SurfaceRef surface, SurfaceCapabilities* pSurfaceCapabilities);
    using PFN_getPhysicalDeviceSurfaceFormatsKHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, SurfaceRef surface, uint32_t* pSurfaceFormatCount, SurfaceFormat* pSurfaceFormats);
    using PFN_getPhysicalDeviceSurfacePresentModesKHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, SurfaceRef surface, uint32_t* pPresentModeCount, PresentMode* pPresentModes);
    using PFN_createDebugUtilsMessengerEXT = Result(VKAPI_PTR*)(InstanceRef instance, const DebugUtils::MessengerCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DebugUtils::Messenger* pMessenger);
    using PFN_destroyDebugUtilsMessengerEXT = void(VKAPI_PTR*)(InstanceRef instance, DebugUtils::MessengerRef messenger, const AllocationCallbacks* pAllocator);
    //using PFN_submitDebugUtilsMessageEXT = void(VKAPI_PTR*)(InstanceRef instance, Flags::DebugMessageSeverity::Bits messageSeverity, Flags::DebugMessageType messageTypes, const DebugUtils::CallbackData* pCallbackData);
    using PFN_setDebugUtilsObjectNameEXT = Result(VKAPI_PTR*)(DeviceRef device, const DebugUtils::ObjectNameInfo* pNameInfo);
    //using PFN_setDebugUtilsObjectTagEXT = Result(VKAPI_PTR*)(DeviceRef device, const DebugUtils::ObjectTagInfo* pTagInfo);
    using PFN_createDebugReportCallbackEXT = Result(VKAPI_PTR*)(InstanceRef instance, const DebugReportCallbackCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DebugReportCallbackRef* pCallback);
    using PFN_destroyDebugReportCallbackEXT = void(VKAPI_PTR*)(InstanceRef instance, DebugReportCallbackRef callback, const AllocationCallbacks* pAllocator);
    //using PFN_debugReportMessageEXT = void(VKAPI_PTR*)(InstanceRef instance, Flags::DebugReportObject::Bits flags, DebugReportObjectType objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);
    //using PFN_getPhysicalDeviceFeatures2 = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, PhysicalDeviceFeatures2* pFeatures);
    //using PFN_getPhysicalDeviceProperties2 = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, PhysicalDeviceProperties2* pProperties);
    //using PFN_getPhysicalDeviceFormatProperties2 = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, Format format, FormatProperties2* pFormatProperties);
    //using PFN_getPhysicalDeviceImageFormatProperties2 = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, const PhysicalDeviceImageFormatInfo2* pImageFormatInfo, ImageFormatProperties2* pImageFormatProperties);
    //using PFN_getPhysicalDeviceQueueFamilyProperties2 = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, QueueFamilyProperties2* pQueueFamilyProperties);
    //using PFN_getPhysicalDeviceMemoryProperties2 = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, PhysicalDeviceMemoryProperties2* pMemoryProperties);
    //using PFN_getPhysicalDeviceSparseImageFormatProperties2 = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, const PhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, SparseImageFormatProperties2* pProperties);
    using PFN_getPhysicalDeviceExternalFenceProperties = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, const PhysicalDeviceExternalFenceInfo* pExternalFenceInfo, ExternalFenceProperties* pExternalFenceProperties);
    using PFN_getPhysicalDeviceExternalSemaphoreProperties = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, const PhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, ExternalSemaphoreProperties* pExternalSemaphoreProperties);
    using PFN_getPhysicalDeviceExternalBufferProperties = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, const PhysicalDeviceExternalBufferInfo* pExternalBufferInfo, ExternalBufferProperties* pExternalBufferProperties);
    //using PFN_getPhysicalDeviceSurfaceCapabilities2KHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, const PhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, SurfaceCapabilities2* pSurfaceCapabilities);
    //using PFN_getPhysicalDeviceSurfaceFormats2KHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, const PhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, SurfaceFormat2* pSurfaceFormats);
    using PFN_enumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, PerformanceCounterKHR* pCounters, PerformanceCounterDescriptionKHR* pCounterDescriptions);
    using PFN_getPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR = void(VKAPI_PTR*)(PhysicalDevice physicalDevice, const PhysicalDeviceQueueFamilyPerformanceQueryCreateInfo* pPerformanceQueryCreateInfo, uint32_t* pNumPasses);
    using PFN_createHeadlessSurfaceEXT = Result(VKAPI_PTR*)(InstanceRef instance, const HeadlessSurfaceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceRef* pSurface);
    using PFN_enumeratePhysicalDeviceGroupsKHR = Result(VKAPI_PTR*)(InstanceRef instance, uint32_t* pPhysicalDeviceGroupCount, PhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties);
    using PFN_getPhysicalDeviceFragmentShadingRatesKHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, PhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates);
    using PFN_getPhysicalDeviceToolPropertiesEXT = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, uint32_t* pToolCount, PhysicalDeviceToolProperties* pToolProperties);
    using PFN_acquireDrmDisplayEXT = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, int32_t drmFd, DisplayKHR display);
    using PFN_getDrmDisplayEXT = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, DisplayKHR* display);
    using PFN_getPhysicalDeviceCooperativeMatrixPropertiesNV = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, PhysicalDeviceCooperativeMatrixPropertiesNV* pProperties);
    using PFN_getPhysicalDeviceOpticalFlowImageFormatsNV = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, const OpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, OpticalFlowImageFormatPropertiesNV* pImageFormatProperties);
    using PFN_getPhysicalDeviceDisplayPropertiesKHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPropertiesKHR* pProperties);
    using PFN_getPhysicalDeviceDisplayPlanePropertiesKHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPlanePropertiesKHR* pProperties);
    using PFN_getDisplayPlaneSupportedDisplaysKHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, DisplayKHR* pDisplays);
    using PFN_getDisplayModePropertiesKHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, DisplayKHR display, uint32_t* pPropertyCount, DisplayModePropertiesKHR* pProperties);
    using PFN_createDisplayModeKHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, DisplayKHR display, const DisplayModeCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, DisplayModeKHR* pMode);
    using PFN_getDisplayPlaneCapabilitiesKHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, DisplayModeKHR mode, uint32_t planeIndex, DisplayPlaneCapabilitiesKHR* pCapabilities);
    using PFN_createDisplayPlaneSurfaceKHR = Result(VKAPI_PTR*)(InstanceRef instance, const DisplaySurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceRef* pSurface);
    using PFN_getPhysicalDevicePresentRectanglesKHR = Result(VKAPI_PTR*)(PhysicalDevice physicalDevice, SurfaceRef surface, uint32_t* pRectCount, Rect2D* pRects);
    using PFN_cmdBeginDebugUtilsLabelEXT = void(VKAPI_PTR*)(CommandBuffer commandBuffer, const DebugUtilsLabelEXT* pLabelInfo);
    using PFN_cmdEndDebugUtilsLabelEXT = void(VKAPI_PTR*)(CommandBuffer commandBuffer);
    using PFN_cmdInsertDebugUtilsLabelEXT = void(VKAPI_PTR*)(CommandBuffer commandBuffer, const DebugUtilsLabelEXT* pLabelInfo);
    using PFN_queueBeginDebugUtilsLabelEXT = void(VKAPI_PTR*)(Queue queue, const DebugUtilsLabelEXT* pLabelInfo);
    using PFN_queueEndDebugUtilsLabelEXT = void(VKAPI_PTR*)(Queue queue);
    using PFN_queueInsertDebugUtilsLabelEXT = void(VKAPI_PTR*)(Queue queue, const DebugUtilsLabelEXT* pLabelInfo);
}