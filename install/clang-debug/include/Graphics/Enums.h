#pragma once
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

namespace Graphics {

    //Enums without a vulkan equivalent

    enum class InstanceFunction : uint32_t {
        CreateDevice,
        DestroyInstance,
        EnumeratePhysicalDevices,
        GetPhysicalDeviceFeatures,
        GetPhysicalDeviceFormatProperties,
        GetPhysicalDeviceImageFormatProperties,
        GetPhysicalDeviceProperties,
        GetPhysicalDeviceQueueFamilyProperties,
        GetPhysicalDeviceMemoryProperties,
        GetPhysicalDeviceSparseImageFormatProperties,
        GetDeviceProcAddr,
        EnumerateDeviceExtensionProperties,
        EnumerateDeviceLayerProperties,

        // VK_KHR_surface
#ifdef _WIN32
        CreateWin32SurfaceKHR,
#else
        CreateXlibSurfaceKHR,
#endif
        DestroySurfaceKHR,
        GetPhysicalDeviceSurfaceSupportKHR,
        GetPhysicalDeviceSurfaceCapabilitiesKHR,
        GetPhysicalDeviceSurfaceFormatsKHR,
        GetPhysicalDeviceSurfacePresentModesKHR,

        // VK_EXT_debug_utils
        CreateDebugUtilsMessengerEXT,
        DestroyDebugUtilsMessengerEXT,
        SubmitDebugUtilsMessageEXT,
        CmdBeginDebugUtilsLabelEXT,
        CmdEndDebugUtilsLabelEXT,
        CmdInsertDebugUtilsLabelEXT,
        SetDebugUtilsObjectNameEXT,
        SetDebugUtilsObjectTagEXT,
        QueueBeginDebugUtilsLabelEXT,
        QueueEndDebugUtilsLabelEXT,
        QueueInsertDebugUtilsLabelEXT,

        // VK_EXT_debug_report (legacy)
        CreateDebugReportCallbackEXT,
        DestroyDebugReportCallbackEXT,
        DebugReportMessageEXT,


        GetPhysicalDeviceFeatures2,
        GetPhysicalDeviceProperties2,
        GetPhysicalDeviceFormatProperties2,
        GetPhysicalDeviceImageFormatProperties2,
        GetPhysicalDeviceQueueFamilyProperties2,
        GetPhysicalDeviceMemoryProperties2,
        GetPhysicalDeviceSparseImageFormatProperties2,

        GetPhysicalDeviceExternalFenceProperties,
        GetPhysicalDeviceExternalSemaphoreProperties,
        GetPhysicalDeviceExternalBufferProperties,

        // VK_KHR_get_physical_device_properties2
        GetPhysicalDeviceFeatures2KHR,
        GetPhysicalDeviceProperties2KHR,
        GetPhysicalDeviceFormatProperties2KHR,
        GetPhysicalDeviceImageFormatProperties2KHR,
        GetPhysicalDeviceQueueFamilyProperties2KHR,
        GetPhysicalDeviceMemoryProperties2KHR,
        GetPhysicalDeviceSparseImageFormatProperties2KHR,

        // VK_KHR_external_fence_capabilities
        GetPhysicalDeviceExternalFencePropertiesKHR,

        // VK_KHR_external_semaphore_capabilities
        GetPhysicalDeviceExternalSemaphorePropertiesKHR,

        // VK_KHR_external_memory_capabilities
        GetPhysicalDeviceExternalBufferPropertiesKHR,

        // VK_KHR_get_surface_capabilities2
        GetPhysicalDeviceSurfaceCapabilities2KHR,
        GetPhysicalDeviceSurfaceFormats2KHR,

        // VK_KHR_performance_query
        EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR,
        GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR,

        // VK_EXT_headless_surface
        CreateHeadlessSurfaceEXT,

        // VK_KHR_device_group_creation
        EnumeratePhysicalDeviceGroupsKHR,

        // VK_KHR_fragment_shading_rate
        GetPhysicalDeviceFragmentShadingRatesKHR,

        // VK_EXT_tooling_info
        GetPhysicalDeviceToolPropertiesEXT,

        // VK_EXT_acquire_drm_display
        AcquireDrmDisplayEXT,
        GetDrmDisplayEXT,

        // VK_NV_cooperative_matrix
        GetPhysicalDeviceCooperativeMatrixPropertiesNV,

        // VK_NV_copy_memory_indirect
        // GetPhysicalDeviceGeneratedCommandsPropertiesNV,

        // VK_NV_optical_flow
        GetPhysicalDeviceOpticalFlowImageFormatsNV,

        // VK_EXT_metal_surface
        // CreateMetalSurfaceEXT,

        // VK_KHR_wayland_surface
        // CreateWaylandSurfaceKHR,
        // GetPhysicalDeviceWaylandPresentationSupportKHR,

        // VK_KHR_win32_surface
        // CreateWin32SurfaceKHR,
        // GetPhysicalDeviceWin32PresentationSupportKHR,

        // VK_KHR_xlib_surface
        // CreateXlibSurfaceKHR,
        // GetPhysicalDeviceXlibPresentationSupportKHR,

        // VK_KHR_xcb_surface
        // CreateXcbSurfaceKHR,
        // GetPhysicalDeviceXcbPresentationSupportKHR,

        // VK_KHR_android_surface
        // CreateAndroidSurfaceKHR,

        // VK_MVK_ios_surface
        // CreateIOSSurfaceMVK,

        // VK_MVK_macos_surface
        // CreateMacOSSurfaceMVK,

        // VK_GGP_stream_descriptor_surface
        // CreateStreamDescriptorSurfaceGGP,

        // VK_NN_vi_surface
        // CreateViSurfaceNN,

        // VK_EXT_full_screen_exclusive
        // GetPhysicalDeviceSurfacePresentModes2EXT,

        // VK_EXT_acquire_xlib_display
        // AcquireXlibDisplayEXT,
        // GetRandROutputDisplayEXT,

        // VK_KHR_display
        GetPhysicalDeviceDisplayPropertiesKHR,
        GetPhysicalDeviceDisplayPlanePropertiesKHR,
        GetDisplayPlaneSupportedDisplaysKHR,
        GetDisplayModePropertiesKHR,
        CreateDisplayModeKHR,
        GetDisplayPlaneCapabilitiesKHR,
        CreateDisplayPlaneSurfaceKHR,

        // VK_KHR_swapchain
        GetPhysicalDevicePresentRectanglesKHR,

        Num
    };

    enum class DeviceFunction : uint32_t {
        // Core device functions
        DestroyDevice,
        GetDeviceQueue,
        GetDeviceQueue2,
        DeviceWaitIdle,
        AllocateMemory,
        FreeMemory,
        MapMemory,
        UnmapMemory,
        FlushMappedMemoryRanges,
        InvalidateMappedMemoryRanges,
        GetDeviceMemoryCommitment,
        BindBufferMemory,
        BindImageMemory,
        GetBufferMemoryRequirements,
        GetImageMemoryRequirements,
        GetImageSparseMemoryRequirements,
        QueueSubmit,
        QueueWaitIdle,
        AllocateCommandBuffers,
        FreeCommandBuffers,
        BeginCommandBuffer,
        EndCommandBuffer,
        ResetCommandBuffer,
        CmdBindPipeline,
        CmdSetViewport,
        CmdSetScissor,
        CmdSetLineWidth,
        CmdSetDepthBias,
        CmdSetBlendConstants,
        CmdSetDepthBounds,
        CmdSetStencilCompareMask,
        CmdSetStencilWriteMask,
        CmdSetStencilReference,
        CmdBindDescriptorSets,
        CmdBindIndexBuffer,
        CmdBindVertexBuffers,
        CmdDraw,
        CmdDrawIndexed,
        CmdDrawIndirect,
        CmdDrawIndexedIndirect,
        CmdDispatch,
        CmdDispatchIndirect,
        CmdCopyBuffer,
        CmdCopyImage,
        CmdBlitImage,
        CmdCopyBufferToImage,
        CmdCopyImageToBuffer,
        CmdUpdateBuffer,
        CmdFillBuffer,
        CmdClearColorImage,
        CmdClearDepthStencilImage,
        CmdClearAttachments,
        CmdResolveImage,
        CmdSetEvent,
        CmdResetEvent,
        CmdWaitEvents,
        CmdPipelineBarrier,
        CmdBeginQuery,
        CmdEndQuery,
        CmdResetQueryPool,
        CmdWriteTimestamp,
        CmdCopyQueryPoolResults,
        CmdPushConstants,
        CmdBeginRenderPass,
        CmdNextSubpass,
        CmdEndRenderPass,
        CmdExecuteCommands,

        // VK_KHR_swapchain extension
        CreateSwapchainKHR,
        DestroySwapchainKHR,
        GetSwapchainImagesKHR,
        AcquireNextImageKHR,
        QueuePresentKHR,

        // VK_KHR_synchronization2 extension
        CmdSetEvent2KHR,
        CmdResetEvent2KHR,
        CmdWaitEvents2KHR,
        CmdPipelineBarrier2KHR,
        CmdWriteTimestamp2KHR,
        QueueSubmit2KHR,

        // VK_EXT_debug_utils extension (device functions)
        CmdBeginDebugUtilsLabelEXT,
        CmdEndDebugUtilsLabelEXT,
        CmdInsertDebugUtilsLabelEXT,
        QueueBeginDebugUtilsLabelEXT,
        QueueEndDebugUtilsLabelEXT,
        QueueInsertDebugUtilsLabelEXT,

        //// VK_KHR_acceleration_structure extension
        //CreateAccelerationStructureKHR,
        //DestroyAccelerationStructureKHR,
        //CmdBuildAccelerationStructuresKHR,
        //CmdBuildAccelerationStructuresIndirectKHR,
        //CmdCopyAccelerationStructureKHR,
        //CmdCopyAccelerationStructureToMemoryKHR,
        //CmdCopyMemoryToAccelerationStructureKHR,
        //WriteAccelerationStructuresPropertiesKHR,

        //// VK_KHR_ray_tracing_pipeline extension
        //CmdTraceRaysKHR,
        //CmdTraceRaysIndirectKHR,
        //GetRayTracingShaderGroupHandlesKHR,
        //GetRayTracingCaptureReplayShaderGroupHandlesKHR,
        //CmdTraceRaysIndirect2KHR,

        //// VK_EXT_mesh_shader extension
        //CmdDrawMeshTasksEXT,
        //CmdDrawMeshTasksIndirectEXT,
        //CmdDrawMeshTasksIndirectCountEXT,

        // VK_KHR_buffer_device_address extension
        GetBufferDeviceAddressKHR,
        GetBufferOpaqueCaptureAddressKHR,
        GetDeviceMemoryOpaqueCaptureAddressKHR,

        //// VK_EXT_descriptor_buffer extension
        //GetDescriptorSetLayoutSizeEXT,
        //GetDescriptorSetLayoutBindingOffsetEXT,
        //GetDescriptorEXT,
        //CmdBindDescriptorBuffersEXT,
        //CmdSetDescriptorBufferOffsetsEXT,
        //CmdBindDescriptorBufferEmbeddedSamplersEXT,
        //GetBufferOpaqueCaptureDescriptorDataEXT,
        //GetImageOpaqueCaptureDescriptorDataEXT,
        //GetImageViewOpaqueCaptureDescriptorDataEXT,
        //GetSamplerOpaqueCaptureDescriptorDataEXT,

        // VK_KHR_dynamic_rendering extension
        CmdBeginRenderingKHR,
        CmdEndRenderingKHR,

        //// VK_EXT_vertex_input_dynamic_state extension
        //CmdSetVertexInputEXT,

        //// VK_EXT_color_write_enable extension
        //CmdSetColorWriteEnableEXT,

        //// VK_EXT_extended_dynamic_state extension
        //CmdSetCullModeEXT,
        //CmdSetFrontFaceEXT,
        //CmdSetPrimitiveTopologyEXT,
        //CmdSetViewportWithCountEXT,
        //CmdSetScissorWithCountEXT,
        //CmdBindVertexBuffers2EXT,
        //CmdSetDepthTestEnableEXT,
        //CmdSetDepthWriteEnableEXT,
        //CmdSetDepthCompareOpEXT,
        //CmdSetDepthBoundsTestEnableEXT,
        //CmdSetStencilTestEnableEXT,
        //CmdSetStencilOpEXT,

        //// VK_EXT_extended_dynamic_state2 extension
        //CmdSetPatchControlPointsEXT,
        //CmdSetRasterizerDiscardEnableEXT,
        //CmdSetDepthBiasEnableEXT,
        //CmdSetLogicOpEXT,
        //CmdSetPrimitiveRestartEnableEXT,

        //// VK_EXT_extended_dynamic_state3 extension
        //CmdSetTessellationDomainOriginEXT,
        //CmdSetDepthClampEnableEXT,
        //CmdSetPolygonModeEXT,
        //CmdSetRasterizationSamplesEXT,
        //CmdSetSampleMaskEXT,
        //CmdSetAlphaToCoverageEnableEXT,
        //CmdSetAlphaToOneEnableEXT,
        //CmdSetLogicOpEnableEXT,
        //CmdSetColorBlendEnableEXT,
        //CmdSetColorBlendEquationEXT,
        //CmdSetColorWriteMaskEXT,

        // VK_KHR_timeline_semaphore extension
        GetSemaphoreCounterValueKHR,
        WaitSemaphoresKHR,
        SignalSemaphoreKHR,

        // VK_KHR_push_descriptor extension
        //CmdPushDescriptorSetKHR,
        //CmdPushDescriptorSetWithTemplateKHR,

        //// VK_EXT_private_data extension
        //CreatePrivateDataSlotEXT,
        //DestroyPrivateDataSlotEXT,
        //SetPrivateDataEXT,
        //GetPrivateDataEXT,

        // VK_KHR_copy_commands2 extension
        CmdCopyBuffer2KHR,
        CmdCopyImage2KHR,
        CmdCopyBufferToImage2KHR,
        CmdCopyImageToBuffer2KHR,
        CmdBlitImage2KHR,
        CmdResolveImage2KHR,

        // VK_KHR_maintenance4 extension
        GetDeviceBufferMemoryRequirementsKHR,
        GetDeviceImageMemoryRequirementsKHR,
        GetDeviceImageSparseMemoryRequirementsKHR,

        WaitForFences,
        ResetFences,

        ResetCommandPool,
        ResetDescriptorPool,

        CreateGraphicsPipelines,
        DestroyPipeline,

        UpdateDescriptorSets,

        DestroyRenderPass,
        CreateRenderPass,
        DestroyBuffer,
        CreateBuffer,
        DestroyImage,
        DestroyImageView,
        CreateImage,
        CreateImageView,
        DestroySemaphore,
        CreateSemaphore,
        DestroyFence,
        CreateFence,
        DestroyShaderModule,
        CreateShaderModule,
        DestroyFramebuffer,
        CreateFramebuffer,
        DestroyCommandPool,
        CreateCommandPool,
        DestroySampler,
        CreateSampler,
        DestroyDescriptorSetLayout,
        CreateDescriptorSetLayout,
        FreeDescriptorSets,
        AllocateDescriptorSets,
        DestroyPipelineLayout,
        CreatePipelineLayout,
        DestroyDescriptorPool,
        CreateDescriptorPool,
        DestroyBufferView,
        CreateBufferView,
        Num
    };

    //Enums with a vulkan equivalent

    template<typename T>
    struct EnumVulkanConnect;

    template<typename T>
    struct EnumCVulkanConnect;

    enum class Result : int32_t {
        Success = VK_SUCCESS,
        NotReady = VK_NOT_READY,
        Timeout = VK_TIMEOUT,
        EventSet = VK_EVENT_SET,
        EventReset = VK_EVENT_RESET,
        Incomplete = VK_INCOMPLETE,
        ErrorOutOfHostMemory = VK_ERROR_OUT_OF_HOST_MEMORY,
        ErrorOutOfDeviceMemory = VK_ERROR_OUT_OF_DEVICE_MEMORY,
        ErrorInitializationFailed = VK_ERROR_INITIALIZATION_FAILED,
        ErrorDeviceLost = VK_ERROR_DEVICE_LOST,
        ErrorMemoryMapFailed = VK_ERROR_MEMORY_MAP_FAILED,
        ErrorLayerNotPresent = VK_ERROR_LAYER_NOT_PRESENT,
        ErrorExtensionNotPresent = VK_ERROR_EXTENSION_NOT_PRESENT,
        ErrorFeatureNotPresent = VK_ERROR_FEATURE_NOT_PRESENT,
        ErrorIncompatibleDriver = VK_ERROR_INCOMPATIBLE_DRIVER,
        ErrorTooManyObjects = VK_ERROR_TOO_MANY_OBJECTS,
        ErrorFormatNotSupported = VK_ERROR_FORMAT_NOT_SUPPORTED,
        ErrorFragmentedPool = VK_ERROR_FRAGMENTED_POOL,
        ErrorUnknown = VK_ERROR_UNKNOWN,
        ErrorOutOfPoolMemory = VK_ERROR_OUT_OF_POOL_MEMORY,
        ErrorInvalidExternalHandle = VK_ERROR_INVALID_EXTERNAL_HANDLE,
        ErrorFragmentation = VK_ERROR_FRAGMENTATION,
        ErrorInvalidOpaqueCaptureAddress = VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,
        PipelineCompileRequired = VK_PIPELINE_COMPILE_REQUIRED,
        ErrorNotPermitted = VK_ERROR_NOT_PERMITTED,
        ErrorSurfaceLostKHR = VK_ERROR_SURFACE_LOST_KHR,
        ErrorNativeWindowInUseKHR = VK_ERROR_NATIVE_WINDOW_IN_USE_KHR,
        SuboptimalKHR = VK_SUBOPTIMAL_KHR,
        ErrorOutOfDateKHR = VK_ERROR_OUT_OF_DATE_KHR,
        ErrorIncompatibleDisplayKHR = VK_ERROR_INCOMPATIBLE_DISPLAY_KHR,
        ErrorValidationFailedEXT = VK_ERROR_VALIDATION_FAILED_EXT,
        ErrorInvalidShaderNV = VK_ERROR_INVALID_SHADER_NV,
        ErrorImageUsageNotSupportedKHR = VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR,
        ErrorVideoPictureLayoutNotSupportedKHR = VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR,
        ErrorVideoProfileOperationNotSupportedKHR = VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR,
        ErrorVideoProfileFormatNotSupportedKHR = VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR,
        ErrorVideoProfileCodecNotSupportedKHR = VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR,
        ErrorVideoStdVersionNotSupportedKHR = VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR,
        ErrorInvalidDrmFormatModifierPlaneLayoutEXT = VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT,
        ErrorFullScreenExclusiveModeLostEXT = VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT,
        ThreadIdleKHR = VK_THREAD_IDLE_KHR,
        ThreadDoneKHR = VK_THREAD_DONE_KHR,
        OperationDeferredKHR = VK_OPERATION_DEFERRED_KHR,
        OperationNotDeferredKHR = VK_OPERATION_NOT_DEFERRED_KHR,
        ErrorInvalidVideoStdParametersKHR = VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR,
        ErrorCompressionExhaustedEXT = VK_ERROR_COMPRESSION_EXHAUSTED_EXT,
        IncompatibleShaderBinaryEXT = VK_INCOMPATIBLE_SHADER_BINARY_EXT,
        PipelineBinaryMissingKHR = VK_PIPELINE_BINARY_MISSING_KHR,
        ErrorNotEnoughSpaceKHR = VK_ERROR_NOT_ENOUGH_SPACE_KHR,

        ErrorOutOfPoolMemoryKHR = VK_ERROR_OUT_OF_POOL_MEMORY_KHR,
        ErrorInvalidExternalHandleKHR = VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR,
        ErrorFragmentationEXT = VK_ERROR_FRAGMENTATION_EXT,
        ErrorNotPermittedEXT = VK_ERROR_NOT_PERMITTED_EXT,
        ErrorNotPermittedKHR = VK_ERROR_NOT_PERMITTED_KHR,
        ErrorInvalidDeviceAddressEXT = VK_ERROR_INVALID_DEVICE_ADDRESS_EXT,
        ErrorInvalidOpaqueCaptureAddressKHR = VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR,
        PipelineCompileRequiredEXT = VK_PIPELINE_COMPILE_REQUIRED_EXT,
        ErrorPipelineCompileRequiredEXT = VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT,
        ErrorIncompatibleShaderBinaryEXT = VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT
    };

    template<>
    struct EnumVulkanConnect<Result> {
        using CorrespondingType = vk::Result;
    };

    template<>
    struct EnumVulkanConnect<vk::Result> {
        using CorrespondingType = Result;
    };

    template<>
    struct EnumCVulkanConnect<Result> {
        using CorrespondingType = VkResult;
    };

    template<>
    struct EnumCVulkanConnect<VkResult> {
        using CorrespondingType = Result;
    };

    enum class DescriptorType : uint32_t
    {
        UniformBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        Sampler = VK_DESCRIPTOR_TYPE_SAMPLER,
        CombinedImageSampler = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        StorageBuffer = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        StorageImage = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        InputAttachment = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
        UniformTexelBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
        StorageTexelBuffer = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
    };

    template<>
    struct EnumVulkanConnect<DescriptorType> {
        using CorrespondingType = VkDescriptorType;
    };

    template<>
    struct EnumVulkanConnect<VkDescriptorType> {
        using CorrespondingType = DescriptorType;
    };

    template<>
    struct EnumCVulkanConnect<DescriptorType> {
        using CorrespondingType = VkDescriptorType;
    };

    template<>
    struct EnumCVulkanConnect<VkDescriptorType> {
        using CorrespondingType = DescriptorType;
    };

    enum class ImageLayout : uint32_t
    {
        Undefined = VK_IMAGE_LAYOUT_UNDEFINED,
        General = VK_IMAGE_LAYOUT_GENERAL,
        ColorAttachmentOptimal = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        DepthStencilAttachmentOptimal = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        DepthStencilReadOnlyOptimal = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
        ShaderReadOnlyOptimal = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        TransferSrcOptimal = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        TransferDstOptimal = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        Preinitialized = VK_IMAGE_LAYOUT_PREINITIALIZED,
        DepthReadOnlyStencilAttachmentOptimal = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL,
        DepthAttachmentStencilReadOnlyOptimal = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,
        DepthAttachmentOptimal = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
        DepthReadOnlyOptimal = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL,
        StencilAttachmentOptimal = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL,
        StencilReadOnlyOptimal = VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL,
        ReadOnlyOptimal = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,
        AttachmentOptimal = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
        PresentSrcKHR = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        VideoDecodeDstKHR = VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR,
        VideoDecodeSrcKHR = VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR,
        VideoDecodeDpbKHR = VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR,
        SharedPresentKHR = VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR,
        FragmentDensityMapOptimalEXT = VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT,
        FragmentShadingRateAttachmentOptimalKHR = VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR,
        AttachmentFeedbackLoopOptimalEXT = VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT,
    };

    template<>
    struct EnumVulkanConnect<ImageLayout> {
        using CorrespondingType = VkImageLayout;
    };

    template<>
    struct EnumVulkanConnect<VkImageLayout> {
        using CorrespondingType = ImageLayout;
    };

    template<>
    struct EnumCVulkanConnect<ImageLayout> {
        using CorrespondingType = VkImageLayout;
    };

    template<>
    struct EnumCVulkanConnect<VkImageLayout> {
        using CorrespondingType = ImageLayout;
    };

    enum class Format : uint32_t
    {
        Undefined = VK_FORMAT_UNDEFINED,
        R4G4UnormPack8 = VK_FORMAT_R4G4_UNORM_PACK8,
        R4G4B4A4UnormPack16 = VK_FORMAT_R4G4B4A4_UNORM_PACK16,
        B4G4R4A4UnormPack16 = VK_FORMAT_B4G4R4A4_UNORM_PACK16,
        R5G6B5UnormPack16 = VK_FORMAT_R5G6B5_UNORM_PACK16,
        B5G6R5UnormPack16 = VK_FORMAT_B5G6R5_UNORM_PACK16,
        R5G5B5A1UnormPack16 = VK_FORMAT_R5G5B5A1_UNORM_PACK16,
        B5G5R5A1UnormPack16 = VK_FORMAT_B5G5R5A1_UNORM_PACK16,
        A1R5G5B5UnormPack16 = VK_FORMAT_A1R5G5B5_UNORM_PACK16,
        R8Unorm = VK_FORMAT_R8_UNORM,
        R8Snorm = VK_FORMAT_R8_SNORM,
        R8Uscaled = VK_FORMAT_R8_USCALED,
        R8Sscaled = VK_FORMAT_R8_SSCALED,
        R8Uint = VK_FORMAT_R8_UINT,
        R8Sint = VK_FORMAT_R8_SINT,
        R8Srgb = VK_FORMAT_R8_SRGB,
        R8G8Unorm = VK_FORMAT_R8G8_UNORM,
        R8G8Snorm = VK_FORMAT_R8G8_SNORM,
        R8G8Uscaled = VK_FORMAT_R8G8_USCALED,
        R8G8Sscaled = VK_FORMAT_R8G8_SSCALED,
        R8G8Uint = VK_FORMAT_R8G8_UINT,
        R8G8Sint = VK_FORMAT_R8G8_SINT,
        R8G8Srgb = VK_FORMAT_R8G8_SRGB,
        R8G8B8Unorm = VK_FORMAT_R8G8B8_UNORM,
        R8G8B8Snorm = VK_FORMAT_R8G8B8_SNORM,
        R8G8B8Uscaled = VK_FORMAT_R8G8B8_USCALED,
        R8G8B8Sscaled = VK_FORMAT_R8G8B8_SSCALED,
        R8G8B8Uint = VK_FORMAT_R8G8B8_UINT,
        R8G8B8Sint = VK_FORMAT_R8G8B8_SINT,
        R8G8B8Srgb = VK_FORMAT_R8G8B8_SRGB,
        B8G8R8Unorm = VK_FORMAT_B8G8R8_UNORM,
        B8G8R8Snorm = VK_FORMAT_B8G8R8_SNORM,
        B8G8R8Uscaled = VK_FORMAT_B8G8R8_USCALED,
        B8G8R8Sscaled = VK_FORMAT_B8G8R8_SSCALED,
        B8G8R8Uint = VK_FORMAT_B8G8R8_UINT,
        B8G8R8Sint = VK_FORMAT_B8G8R8_SINT,
        B8G8R8Srgb = VK_FORMAT_B8G8R8_SRGB,
        R8G8B8A8Unorm = VK_FORMAT_R8G8B8A8_UNORM,
        R8G8B8A8Snorm = VK_FORMAT_R8G8B8A8_SNORM,
        R8G8B8A8Uscaled = VK_FORMAT_R8G8B8A8_USCALED,
        R8G8B8A8Sscaled = VK_FORMAT_R8G8B8A8_SSCALED,
        R8G8B8A8Uint = VK_FORMAT_R8G8B8A8_UINT,
        R8G8B8A8Sint = VK_FORMAT_R8G8B8A8_SINT,
        R8G8B8A8Srgb = VK_FORMAT_R8G8B8A8_SRGB,
        B8G8R8A8Unorm = VK_FORMAT_B8G8R8A8_UNORM,
        B8G8R8A8Snorm = VK_FORMAT_B8G8R8A8_SNORM,
        B8G8R8A8Uscaled = VK_FORMAT_B8G8R8A8_USCALED,
        B8G8R8A8Sscaled = VK_FORMAT_B8G8R8A8_SSCALED,
        B8G8R8A8Uint = VK_FORMAT_B8G8R8A8_UINT,
        B8G8R8A8Sint = VK_FORMAT_B8G8R8A8_SINT,
        B8G8R8A8Srgb = VK_FORMAT_B8G8R8A8_SRGB,
        A8B8G8R8UnormPack32 = VK_FORMAT_A8B8G8R8_UNORM_PACK32,
        A8B8G8R8SnormPack32 = VK_FORMAT_A8B8G8R8_SNORM_PACK32,
        A8B8G8R8UscaledPack32 = VK_FORMAT_A8B8G8R8_USCALED_PACK32,
        A8B8G8R8SscaledPack32 = VK_FORMAT_A8B8G8R8_SSCALED_PACK32,
        A8B8G8R8UintPack32 = VK_FORMAT_A8B8G8R8_UINT_PACK32,
        A8B8G8R8SintPack32 = VK_FORMAT_A8B8G8R8_SINT_PACK32,
        A8B8G8R8SrgbPack32 = VK_FORMAT_A8B8G8R8_SRGB_PACK32,
        A2R10G10B10UnormPack32 = VK_FORMAT_A2R10G10B10_UNORM_PACK32,
        A2R10G10B10SnormPack32 = VK_FORMAT_A2R10G10B10_SNORM_PACK32,
        A2R10G10B10UscaledPack32 = VK_FORMAT_A2R10G10B10_USCALED_PACK32,
        A2R10G10B10SscaledPack32 = VK_FORMAT_A2R10G10B10_SSCALED_PACK32,
        A2R10G10B10UintPack32 = VK_FORMAT_A2R10G10B10_UINT_PACK32,
        A2R10G10B10SintPack32 = VK_FORMAT_A2R10G10B10_SINT_PACK32,
        A2B10G10R10UnormPack32 = VK_FORMAT_A2B10G10R10_UNORM_PACK32,
        A2B10G10R10SnormPack32 = VK_FORMAT_A2B10G10R10_SNORM_PACK32,
        A2B10G10R10UscaledPack32 = VK_FORMAT_A2B10G10R10_USCALED_PACK32,
        A2B10G10R10SscaledPack32 = VK_FORMAT_A2B10G10R10_SSCALED_PACK32,
        A2B10G10R10UintPack32 = VK_FORMAT_A2B10G10R10_UINT_PACK32,
        A2B10G10R10SintPack32 = VK_FORMAT_A2B10G10R10_SINT_PACK32,
        R16Unorm = VK_FORMAT_R16_UNORM,
        R16Snorm = VK_FORMAT_R16_SNORM,
        R16Uscaled = VK_FORMAT_R16_USCALED,
        R16Sscaled = VK_FORMAT_R16_SSCALED,
        R16Uint = VK_FORMAT_R16_UINT,
        R16Sint = VK_FORMAT_R16_SINT,
        R16Sfloat = VK_FORMAT_R16_SFLOAT,
        R16G16Unorm = VK_FORMAT_R16G16_UNORM,
        R16G16Snorm = VK_FORMAT_R16G16_SNORM,
        R16G16Uscaled = VK_FORMAT_R16G16_USCALED,
        R16G16Sscaled = VK_FORMAT_R16G16_SSCALED,
        R16G16Uint = VK_FORMAT_R16G16_UINT,
        R16G16Sint = VK_FORMAT_R16G16_SINT,
        R16G16Sfloat = VK_FORMAT_R16G16_SFLOAT,
        R16G16B16Unorm = VK_FORMAT_R16G16B16_UNORM,
        R16G16B16Snorm = VK_FORMAT_R16G16B16_SNORM,
        R16G16B16Uscaled = VK_FORMAT_R16G16B16_USCALED,
        R16G16B16Sscaled = VK_FORMAT_R16G16B16_SSCALED,
        R16G16B16Uint = VK_FORMAT_R16G16B16_UINT,
        R16G16B16Sint = VK_FORMAT_R16G16B16_SINT,
        R16G16B16Sfloat = VK_FORMAT_R16G16B16_SFLOAT,
        R16G16B16A16Unorm = VK_FORMAT_R16G16B16A16_UNORM,
        R16G16B16A16Snorm = VK_FORMAT_R16G16B16A16_SNORM,
        R16G16B16A16Uscaled = VK_FORMAT_R16G16B16A16_USCALED,
        R16G16B16A16Sscaled = VK_FORMAT_R16G16B16A16_SSCALED,
        R16G16B16A16Uint = VK_FORMAT_R16G16B16A16_UINT,
        R16G16B16A16Sint = VK_FORMAT_R16G16B16A16_SINT,
        R16G16B16A16Sfloat = VK_FORMAT_R16G16B16A16_SFLOAT,
        R32Uint = VK_FORMAT_R32_UINT,
        R32Sint = VK_FORMAT_R32_SINT,
        R32Sfloat = VK_FORMAT_R32_SFLOAT,
        R32G32Uint = VK_FORMAT_R32G32_UINT,
        R32G32Sint = VK_FORMAT_R32G32_SINT,
        R32G32Sfloat = VK_FORMAT_R32G32_SFLOAT,
        R32G32B32Uint = VK_FORMAT_R32G32B32_UINT,
        R32G32B32Sint = VK_FORMAT_R32G32B32_SINT,
        R32G32B32Sfloat = VK_FORMAT_R32G32B32_SFLOAT,
        R32G32B32A32Uint = VK_FORMAT_R32G32B32A32_UINT,
        R32G32B32A32Sint = VK_FORMAT_R32G32B32A32_SINT,
        R32G32B32A32Sfloat = VK_FORMAT_R32G32B32A32_SFLOAT,
        R64Uint = VK_FORMAT_R64_UINT,
        R64Sint = VK_FORMAT_R64_SINT,
        R64Sfloat = VK_FORMAT_R64_SFLOAT,
        R64G64Uint = VK_FORMAT_R64G64_UINT,
        R64G64Sint = VK_FORMAT_R64G64_SINT,
        R64G64Sfloat = VK_FORMAT_R64G64_SFLOAT,
        R64G64B64Uint = VK_FORMAT_R64G64B64_UINT,
        R64G64B64Sint = VK_FORMAT_R64G64B64_SINT,
        R64G64B64Sfloat = VK_FORMAT_R64G64B64_SFLOAT,
        R64G64B64A64Uint = VK_FORMAT_R64G64B64A64_UINT,
        R64G64B64A64Sint = VK_FORMAT_R64G64B64A64_SINT,
        R64G64B64A64Sfloat = VK_FORMAT_R64G64B64A64_SFLOAT,
        B10G11R11UfloatPack32 = VK_FORMAT_B10G11R11_UFLOAT_PACK32,
        E5B9G9R9UfloatPack32 = VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
        D16Unorm = VK_FORMAT_D16_UNORM,
        X8D24UnormPack32 = VK_FORMAT_X8_D24_UNORM_PACK32,
        D32Sfloat = VK_FORMAT_D32_SFLOAT,
        S8Uint = VK_FORMAT_S8_UINT,
        D16UnormS8Uint = VK_FORMAT_D16_UNORM_S8_UINT,
        D24UnormS8Uint = VK_FORMAT_D24_UNORM_S8_UINT,
        D32SfloatS8Uint = VK_FORMAT_D32_SFLOAT_S8_UINT,
        Bc1RgbUnormBlock = VK_FORMAT_BC1_RGB_UNORM_BLOCK,
        Bc1RgbSrgbBlock = VK_FORMAT_BC1_RGB_SRGB_BLOCK,
        Bc1RgbaUnormBlock = VK_FORMAT_BC1_RGBA_UNORM_BLOCK,
        Bc1RgbaSrgbBlock = VK_FORMAT_BC1_RGBA_SRGB_BLOCK,
        Bc2UnormBlock = VK_FORMAT_BC2_UNORM_BLOCK,
        Bc2SrgbBlock = VK_FORMAT_BC2_SRGB_BLOCK,
        Bc3UnormBlock = VK_FORMAT_BC3_UNORM_BLOCK,
        Bc3SrgbBlock = VK_FORMAT_BC3_SRGB_BLOCK,
        Bc4UnormBlock = VK_FORMAT_BC4_UNORM_BLOCK,
        Bc4SnormBlock = VK_FORMAT_BC4_SNORM_BLOCK,
        Bc5UnormBlock = VK_FORMAT_BC5_UNORM_BLOCK,
        Bc5SnormBlock = VK_FORMAT_BC5_SNORM_BLOCK,
        Bc6HUfloatBlock = VK_FORMAT_BC6H_UFLOAT_BLOCK,
        Bc6HSfloatBlock = VK_FORMAT_BC6H_SFLOAT_BLOCK,
        Bc7UnormBlock = VK_FORMAT_BC7_UNORM_BLOCK,
        Bc7SrgbBlock = VK_FORMAT_BC7_SRGB_BLOCK,
        Etc2R8G8B8UnormBlock = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK,
        Etc2R8G8B8SrgbBlock = VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK,
        Etc2R8G8B8A1UnormBlock = VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,
        Etc2R8G8B8A1SrgbBlock = VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,
        Etc2R8G8B8A8UnormBlock = VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK,
        Etc2R8G8B8A8SrgbBlock = VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK,
        EacR11UnormBlock = VK_FORMAT_EAC_R11_UNORM_BLOCK,
        EacR11SnormBlock = VK_FORMAT_EAC_R11_SNORM_BLOCK,
        EacR11G11UnormBlock = VK_FORMAT_EAC_R11G11_UNORM_BLOCK,
        EacR11G11SnormBlock = VK_FORMAT_EAC_R11G11_SNORM_BLOCK,
        Astc4x4UnormBlock = VK_FORMAT_ASTC_4x4_UNORM_BLOCK,
        Astc4x4SrgbBlock = VK_FORMAT_ASTC_4x4_SRGB_BLOCK,
        Astc5x4UnormBlock = VK_FORMAT_ASTC_5x4_UNORM_BLOCK,
        Astc5x4SrgbBlock = VK_FORMAT_ASTC_5x4_SRGB_BLOCK,
        Astc5x5UnormBlock = VK_FORMAT_ASTC_5x5_UNORM_BLOCK,
        Astc5x5SrgbBlock = VK_FORMAT_ASTC_5x5_SRGB_BLOCK,
        Astc6x5UnormBlock = VK_FORMAT_ASTC_6x5_UNORM_BLOCK,
        Astc6x5SrgbBlock = VK_FORMAT_ASTC_6x5_SRGB_BLOCK,
        Astc6x6UnormBlock = VK_FORMAT_ASTC_6x6_UNORM_BLOCK,
        Astc6x6SrgbBlock = VK_FORMAT_ASTC_6x6_SRGB_BLOCK,
        Astc8x5UnormBlock = VK_FORMAT_ASTC_8x5_UNORM_BLOCK,
        Astc8x5SrgbBlock = VK_FORMAT_ASTC_8x5_SRGB_BLOCK,
        Astc8x6UnormBlock = VK_FORMAT_ASTC_8x6_UNORM_BLOCK,
        Astc8x6SrgbBlock = VK_FORMAT_ASTC_8x6_SRGB_BLOCK,
        Astc8x8UnormBlock = VK_FORMAT_ASTC_8x8_UNORM_BLOCK,
        Astc8x8SrgbBlock = VK_FORMAT_ASTC_8x8_SRGB_BLOCK,
        Astc10x5UnormBlock = VK_FORMAT_ASTC_10x5_UNORM_BLOCK,
        Astc10x5SrgbBlock = VK_FORMAT_ASTC_10x5_SRGB_BLOCK,
        Astc10x6UnormBlock = VK_FORMAT_ASTC_10x6_UNORM_BLOCK,
        Astc10x6SrgbBlock = VK_FORMAT_ASTC_10x6_SRGB_BLOCK,
        Astc10x8UnormBlock = VK_FORMAT_ASTC_10x8_UNORM_BLOCK,
        Astc10x8SrgbBlock = VK_FORMAT_ASTC_10x8_SRGB_BLOCK,
        Astc10x10UnormBlock = VK_FORMAT_ASTC_10x10_UNORM_BLOCK,
        Astc10x10SrgbBlock = VK_FORMAT_ASTC_10x10_SRGB_BLOCK,
        Astc12x10UnormBlock = VK_FORMAT_ASTC_12x10_UNORM_BLOCK,
        Astc12x10SrgbBlock = VK_FORMAT_ASTC_12x10_SRGB_BLOCK,
        Astc12x12UnormBlock = VK_FORMAT_ASTC_12x12_UNORM_BLOCK,
        Astc12x12SrgbBlock = VK_FORMAT_ASTC_12x12_SRGB_BLOCK,
        G8B8G8R8422Unorm = VK_FORMAT_G8B8G8R8_422_UNORM,
        B8G8R8G8422Unorm = VK_FORMAT_B8G8R8G8_422_UNORM,
        G8B8R83Plane420Unorm = VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM,
        G8B8R82Plane420Unorm = VK_FORMAT_G8_B8R8_2PLANE_420_UNORM,
        G8B8R83Plane422Unorm = VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM,
        G8B8R82Plane422Unorm = VK_FORMAT_G8_B8R8_2PLANE_422_UNORM,
        G8B8R83Plane444Unorm = VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM,
        R10X6UnormPack16 = VK_FORMAT_R10X6_UNORM_PACK16,
        R10X6G10X6Unorm2Pack16 = VK_FORMAT_R10X6G10X6_UNORM_2PACK16,
        R10X6G10X6B10X6A10X6Unorm4Pack16 = VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16,
        G10X6B10X6G10X6R10X6422Unorm4Pack16 = VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,
        B10X6G10X6R10X6G10X6422Unorm4Pack16 = VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,
        G10X6B10X6R10X63Plane420Unorm3Pack16 = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
        G10X6B10X6R10X62Plane420Unorm3Pack16 = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
        G10X6B10X6R10X63Plane422Unorm3Pack16 = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
        G10X6B10X6R10X62Plane422Unorm3Pack16 = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
        G10X6B10X6R10X63Plane444Unorm3Pack16 = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
        R12X4UnormPack16 = VK_FORMAT_R12X4_UNORM_PACK16,
        R12X4G12X4Unorm2Pack16 = VK_FORMAT_R12X4G12X4_UNORM_2PACK16,
        R12X4G12X4B12X4A12X4Unorm4Pack16 = VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16,
        G12X4B12X4G12X4R12X4422Unorm4Pack16 = VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,
        B12X4G12X4R12X4G12X4422Unorm4Pack16 = VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,
        //G12X4B12X4R12X63Plane420Unorm3Pack16 = VK_FORMAT_G12X4_B12X4_R12X6_3PLANE_420_UNORM_3PACK16,
        //G12X4B12X4R12X62Plane420Unorm3Pack16 = VK_FORMAT_G12X4_B12X4R12X6_2PLANE_420_UNORM_3PACK16,
        //G12X4B12X4R12X63Plane422Unorm3Pack16 = VK_FORMAT_G12X4_B12X4_R12X6_3PLANE_422_UNORM_3PACK16,
        //G12X4B12X4R12X62Plane422Unorm3Pack16 = VK_FORMAT_G12X4_B12X4R12X6_2PLANE_422_UNORM_3PACK16,
        //G12X4B12X4R12X63Plane444Unorm3Pack16 = VK_FORMAT_G12X4_B12X4_R12X6_3PLANE_444_UNORM_3PACK16,
        G16B16G16R16422Unorm = VK_FORMAT_G16B16G16R16_422_UNORM,
        B16G16R16G16422Unorm = VK_FORMAT_B16G16R16G16_422_UNORM,
        G16B16R163Plane420Unorm = VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM,
        G16B16R162Plane420Unorm = VK_FORMAT_G16_B16R16_2PLANE_420_UNORM,
        G16B16R163Plane422Unorm = VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM,
        G16B16R162Plane422Unorm = VK_FORMAT_G16_B16R16_2PLANE_422_UNORM,
        G16B16R163Plane444Unorm = VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM,
        Pvrtc12BppUnormBlockIMG = VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG,
        Pvrtc14BppUnormBlockIMG = VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG,
        Pvrtc22BppUnormBlockIMG = VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG,
        Pvrtc24BppUnormBlockIMG = VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG,
        Pvrtc12BppSrgbBlockIMG = VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG,
        Pvrtc14BppSrgbBlockIMG = VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG,
        Pvrtc22BppSrgbBlockIMG = VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG,
        Pvrtc24BppSrgbBlockIMG = VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG,
        Astc4x4SfloatBlockEXT = VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT,
        Astc5x4SfloatBlockEXT = VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT,
        Astc5x5SfloatBlockEXT = VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT,
        Astc6x5SfloatBlockEXT = VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT,
        Astc6x6SfloatBlockEXT = VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT,
        Astc8x5SfloatBlockEXT = VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT,
        Astc8x6SfloatBlockEXT = VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT,
        Astc8x8SfloatBlockEXT = VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT,
        Astc10x5SfloatBlockEXT = VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT,
        Astc10x6SfloatBlockEXT = VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT,
        Astc10x8SfloatBlockEXT = VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT,
        Astc10x10SfloatBlockEXT = VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT,
        Astc12x10SfloatBlockEXT = VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT,
        Astc12x12SfloatBlockEXT = VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT,
        A4R4G4B4UnormPack16EXT = VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT,
        A4B4G4R4UnormPack16EXT = VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT,
    };

    template<>
    struct EnumVulkanConnect<Format> {
        using CorrespondingType = vk::Format;
    };

    template<>
    struct EnumVulkanConnect<vk::Format> {
        using CorrespondingType = Format;
    };

    template<>
    struct EnumCVulkanConnect<Format> {
        using CorrespondingType = VkFormat;
    };

    template<>
    struct EnumCVulkanConnect<VkFormat> {
        using CorrespondingType = Format;
    };

    enum class Filter : uint32_t
    {
        Linear = VK_FILTER_LINEAR,
        Nearest = VK_FILTER_NEAREST,
        CubicEXT = VK_FILTER_CUBIC_EXT,
        CubicIMG = VK_FILTER_CUBIC_IMG,
    };

    template<>
    struct EnumVulkanConnect<Filter> {
        using CorrespondingType = vk::Filter;
    };

    template<>
    struct EnumVulkanConnect<vk::Filter> {
        using CorrespondingType = Filter;
    };

    template<>
    struct EnumCVulkanConnect<Filter> {
        using CorrespondingType = VkFilter;
    };

    template<>
    struct EnumCVulkanConnect<VkFilter> {
        using CorrespondingType = Filter;
    };

    enum class SamplerAddressMode : uint32_t
    {
        Repeat               = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        MirroredRepeat       = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
        ClampToEdge          = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        ClampToBorder        = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
        MirrorClampToEdge    = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
        MirrorClampToEdgeKHR = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR,
    };

    template<>
    struct EnumVulkanConnect<SamplerAddressMode> {
        using CorrespondingType = vk::SamplerAddressMode;
    };

    template<>
    struct EnumVulkanConnect<vk::SamplerAddressMode> {
        using CorrespondingType = SamplerAddressMode;
    };

    template<>
    struct EnumCVulkanConnect<SamplerAddressMode> {
        using CorrespondingType = VkSamplerAddressMode;
    };

    template<>
    struct EnumCVulkanConnect<VkSamplerAddressMode> {
        using CorrespondingType = SamplerAddressMode;
    };

    enum class SamplerMipmapMode : uint32_t
    {
        Nearest = VK_SAMPLER_MIPMAP_MODE_NEAREST,
        Linear  = VK_SAMPLER_MIPMAP_MODE_LINEAR
    };

    template<>
    struct EnumVulkanConnect<SamplerMipmapMode> {
        using CorrespondingType = vk::SamplerMipmapMode;
    };

    template<>
    struct EnumVulkanConnect<vk::SamplerMipmapMode> {
        using CorrespondingType = SamplerMipmapMode;
    };

    template<>
    struct EnumCVulkanConnect<SamplerMipmapMode> {
        using CorrespondingType = VkSamplerMipmapMode;
    };

    template<>
    struct EnumCVulkanConnect<VkSamplerMipmapMode> {
        using CorrespondingType = SamplerMipmapMode;
    };

    enum class CommandBufferLevel : uint32_t
    {
        Primary   = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        Secondary = VK_COMMAND_BUFFER_LEVEL_SECONDARY
    };

    template<>
    struct EnumVulkanConnect<CommandBufferLevel> {
        using CorrespondingType = vk::CommandBufferLevel;
    };

    template<>
    struct EnumVulkanConnect<vk::CommandBufferLevel> {
        using CorrespondingType = CommandBufferLevel;
    };

    template<>
    struct EnumCVulkanConnect<CommandBufferLevel> {
        using CorrespondingType = VkCommandBufferLevel;
    };

    template<>
    struct EnumCVulkanConnect<VkCommandBufferLevel> {
        using CorrespondingType = CommandBufferLevel;
    };

    enum class ImageTiling : uint32_t
    {
        Optimal              = VK_IMAGE_TILING_OPTIMAL,
        Linear               = VK_IMAGE_TILING_LINEAR,
        DrmFormatModifierEXT = VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT
    };

    template<>
    struct EnumVulkanConnect<ImageTiling> {
        using CorrespondingType = vk::ImageTiling;
    };

    template<>
    struct EnumVulkanConnect<vk::ImageTiling> {
        using CorrespondingType = ImageTiling;
    };

    template<>
    struct EnumCVulkanConnect<ImageTiling> {
        using CorrespondingType = VkImageTiling;
    };

    template<>
    struct EnumCVulkanConnect<VkImageTiling> {
        using CorrespondingType = ImageTiling;
    };
        
    enum class PresentMode : uint32_t
    {
        Immediate               = VK_PRESENT_MODE_IMMEDIATE_KHR,
        Mailbox                 = VK_PRESENT_MODE_MAILBOX_KHR,
        Fifo                    = VK_PRESENT_MODE_FIFO_KHR,
        FifoRelaxed             = VK_PRESENT_MODE_FIFO_RELAXED_KHR,
        SharedDemandRefresh     = VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR,
        SharedContinuousRefresh = VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR,
        FifoLatestReadyEXT      = VK_PRESENT_MODE_FIFO_LATEST_READY_EXT
    };

    template<>
    struct EnumVulkanConnect<PresentMode> {
        using CorrespondingType = vk::PresentModeKHR;
    };

    template<>
    struct EnumVulkanConnect<vk::PresentModeKHR> {
        using CorrespondingType = PresentMode;
    };

    template<>
    struct EnumCVulkanConnect<PresentMode> {
        using CorrespondingType = VkPresentModeKHR;
    };

    template<>
    struct EnumCVulkanConnect<VkPresentModeKHR> {
        using CorrespondingType = PresentMode;
    };

    enum class SharingMode : uint32_t
    {
        Exclusive  = VK_SHARING_MODE_EXCLUSIVE,
        Concurrent = VK_SHARING_MODE_CONCURRENT
    };

    template<>
    struct EnumVulkanConnect<SharingMode> {
        using CorrespondingType = vk::SharingMode;
    };

    template<>
    struct EnumVulkanConnect<vk::SharingMode> {
        using CorrespondingType = SharingMode;
    };

    template<>
    struct EnumCVulkanConnect<SharingMode> {
        using CorrespondingType = VkSharingMode;
    };

    template<>
    struct EnumCVulkanConnect<VkSharingMode> {
        using CorrespondingType = SharingMode;
    };

    enum class ObjectType : uint32_t
    {
        Unknown = VK_OBJECT_TYPE_UNKNOWN,
        Instance = VK_OBJECT_TYPE_INSTANCE,
        PhysicalDevice = VK_OBJECT_TYPE_PHYSICAL_DEVICE,
        Device = VK_OBJECT_TYPE_DEVICE,
        Queue = VK_OBJECT_TYPE_QUEUE,
        Semaphore = VK_OBJECT_TYPE_SEMAPHORE,
        CommandBuffer = VK_OBJECT_TYPE_COMMAND_BUFFER,
        Fence = VK_OBJECT_TYPE_FENCE,
        DeviceMemory = VK_OBJECT_TYPE_DEVICE_MEMORY,
        Buffer = VK_OBJECT_TYPE_BUFFER,
        Image = VK_OBJECT_TYPE_IMAGE,
        Event = VK_OBJECT_TYPE_EVENT,
        QueryPool = VK_OBJECT_TYPE_QUERY_POOL,
        BufferView = VK_OBJECT_TYPE_BUFFER_VIEW,
        ImageView = VK_OBJECT_TYPE_IMAGE_VIEW,
        ShaderModule = VK_OBJECT_TYPE_SHADER_MODULE,
        PipelineCache = VK_OBJECT_TYPE_PIPELINE_CACHE,
        PipelineLayout = VK_OBJECT_TYPE_PIPELINE_LAYOUT,
        RenderPass = VK_OBJECT_TYPE_RENDER_PASS,
        Pipeline = VK_OBJECT_TYPE_PIPELINE,
        DescriptorSetLayout = VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT,
        Sampler = VK_OBJECT_TYPE_SAMPLER,
        DescriptorPool = VK_OBJECT_TYPE_DESCRIPTOR_POOL,
        DescriptorSet = VK_OBJECT_TYPE_DESCRIPTOR_SET,
        Framebuffer = VK_OBJECT_TYPE_FRAMEBUFFER,
        CommandPool = VK_OBJECT_TYPE_COMMAND_POOL,
        SurfaceKHR = VK_OBJECT_TYPE_SURFACE_KHR,
        SwapchainKHR = VK_OBJECT_TYPE_SWAPCHAIN_KHR,
        DisplayKHR = VK_OBJECT_TYPE_DISPLAY_KHR,
        DisplayModeKHR = VK_OBJECT_TYPE_DISPLAY_MODE_KHR,
        DebugReportCallbackEXT = VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT,
        DebugUtilsMessengerEXT = VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT,
        ValidationCacheEXT = VK_OBJECT_TYPE_VALIDATION_CACHE_EXT,
        AccelerationStructureKHR = VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR,
        AccelerationStructureNV = VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV,
        PerformanceConfigurationINTEL = VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL,
        DeferredOperationKHR = VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR,
        IndirectCommandsLayoutNV = VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV,
        PrivateDataSlot = VK_OBJECT_TYPE_PRIVATE_DATA_SLOT,
        VideoSessionKHR = VK_OBJECT_TYPE_VIDEO_SESSION_KHR,
        VideoSessionParametersKHR = VK_OBJECT_TYPE_VIDEO_SESSION_PARAMETERS_KHR,
        CuModuleNVX = VK_OBJECT_TYPE_CU_MODULE_NVX,
        CuFunctionNVX = VK_OBJECT_TYPE_CU_FUNCTION_NVX,
        OpticalFlowSessionNV = VK_OBJECT_TYPE_OPTICAL_FLOW_SESSION_NV,
        MicromapEXT = VK_OBJECT_TYPE_MICROMAP_EXT,
        ShaderEXT = VK_OBJECT_TYPE_SHADER_EXT
    };

    template<>
    struct EnumVulkanConnect<ObjectType> {
        using CorrespondingType = vk::ObjectType;
    };

    template<>
    struct EnumVulkanConnect<vk::ObjectType> {
        using CorrespondingType = ObjectType;
    };

    template<>
    struct EnumCVulkanConnect<ObjectType> {
        using CorrespondingType = VkObjectType;
    };

    template<>
    struct EnumCVulkanConnect<VkObjectType> {
        using CorrespondingType = ObjectType;
    };

    enum class StructureType : uint32_t
    {
        ApplicationInfo = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        InstanceCreateInfo = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        DeviceQueueCreateInfo = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        DeviceCreateInfo = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        SubmitInfo = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        MemoryAllocateInfo = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        MappedMemoryRange = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
        BindSparseInfo = VK_STRUCTURE_TYPE_BIND_SPARSE_INFO,
        FenceCreateInfo = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        SemaphoreCreateInfo = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        EventCreateInfo = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO,
        QueryPoolCreateInfo = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
        BufferCreateInfo = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        BufferViewCreateInfo = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,
        ImageCreateInfo = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        ImageViewCreateInfo = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        ShaderModuleCreateInfo = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        PipelineCacheCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
        PipelineShaderStageCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        PipelineVertexInputStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        PipelineInputAssemblyStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        PipelineTessellationStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
        PipelineViewportStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        PipelineRasterizationStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        PipelineMultisampleStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        PipelineDepthStencilStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        PipelineColorBlendStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        PipelineDynamicStateCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        PipelineRenderingCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        GraphicsPipelineCreateInfo = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        ComputePipelineCreateInfo = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
        PipelineLayoutCreateInfo = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        SamplerCreateInfo = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        DescriptorSetLayoutCreateInfo = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        DescriptorPoolCreateInfo = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        DescriptorSetAllocateInfo = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        WriteDescriptorSet = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        CopyDescriptorSet = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET,
        FramebufferCreateInfo = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        RenderPassCreateInfo = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        CommandPoolCreateInfo = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        CommandBufferAllocateInfo = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        CommandBufferInheritanceInfo = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
        CommandBufferBeginInfo = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        RenderPassBeginInfo = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        BufferMemoryBarrier = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
        ImageMemoryBarrier = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        MemoryBarrier = VK_STRUCTURE_TYPE_MEMORY_BARRIER,
        SwapchainCreateInfoKHR = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        PresentInfoKHR = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        DebugUtilsObjectNameInfoEXT = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
        DebugUtilsObjectTagInfoEXT = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT,
        DebugUtilsLabelEXT = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
        DebugUtilsMessengerCallbackDataEXT = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT,
        DebugUtilsMessengerCreateInfoEXT = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        AccelerationStructureBuildGeometryInfoKHR = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR,

        //device property structs
        PhysicalDeviceProperties2 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
        PhysicalDeviceFeatures2 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        PhysicalDeviceMemoryProperties2 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2,
        PhysicalDeviceVulkan11Features = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
        PhysicalDeviceVulkan11Properties = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES,
        PhysicalDeviceVulkan12Features = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        PhysicalDeviceVulkan12Properties = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES,
        PhysicalDeviceVulkan13Features = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        PhysicalDeviceVulkan13Properties = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES,
        PhysicalDevice16BitStorageFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES,
        PhysicalDeviceMultiviewFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES,
        PhysicalDeviceMultiviewProperties = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES,
        PhysicalDeviceVariablePointersFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES,
        PhysicalDeviceProtectedMemoryFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_FEATURES,
        PhysicalDeviceProtectedMemoryProperties = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_PROPERTIES,
        PhysicalDeviceSamplerYcbcrConversionFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES,
        PhysicalDeviceShaderDrawParametersFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES,
        PhysicalDevice8BitStorageFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES,
        PhysicalDeviceShaderAtomicInt64Features = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES,
        PhysicalDeviceShaderFloat16Int8Features = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES,
        PhysicalDeviceFloatControlsProperties = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES,
        PhysicalDeviceDescriptorIndexingFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES,
        PhysicalDeviceDescriptorIndexingProperties = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES,
        PhysicalDeviceScalarBlockLayoutFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES,
        PhysicalDeviceVulkanMemoryModelFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES,
        PhysicalDeviceImagelessFramebufferFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGELESS_FRAMEBUFFER_FEATURES,
        PhysicalDeviceUniformBufferStandardLayoutFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES,
        PhysicalDeviceShaderSubgroupExtendedTypesFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_EXTENDED_TYPES_FEATURES,
        PhysicalDeviceSeparateDepthStencilLayoutsFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES,
        PhysicalDeviceHostQueryResetFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES,
        PhysicalDeviceTimelineSemaphoreFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES,
        PhysicalDeviceTimelineSemaphoreProperties = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES,
        PhysicalDeviceBufferDeviceAddressFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES,
        PhysicalDeviceBufferDeviceAddressFeaturesEXT = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_EXT,
        PhysicalDeviceImageRobustnessFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ROBUSTNESS_FEATURES,
        PhysicalDeviceInlineUniformBlockFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES,
        PhysicalDeviceInlineUniformBlockProperties = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES,
        PhysicalDevicePipelineCreationCacheControlFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_CREATION_CACHE_CONTROL_FEATURES,
        PhysicalDevicePrivateDataFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIVATE_DATA_FEATURES,
        PhysicalDeviceShaderDemoteToHelperInvocationFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES,
        PhysicalDeviceShaderTerminateInvocationFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TERMINATE_INVOCATION_FEATURES,
        PhysicalDeviceSubgroupSizeControlFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES,
        PhysicalDeviceSubgroupSizeControlProperties = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES,
        PhysicalDeviceSynchronization2Features = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES,
        PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ZERO_INITIALIZE_WORKGROUP_MEMORY_FEATURES,
        PhysicalDeviceDynamicRenderingFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES,
        PhysicalDeviceShaderIntegerDotProductFeatures = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_FEATURES,
        PhysicalDeviceShaderIntegerDotProductProperties = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_PROPERTIES,
        PhysicalDeviceMaintenance4Features = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES,
        PhysicalDeviceMaintenance4Properties = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_PROPERTIES,
        PhysicalDeviceMeshShaderFeaturesEXT = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT,
        PhysicalDeviceMeshShaderFeaturesNV = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV,
        PhysicalDeviceRobustness2FeaturesEXT = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT,

        //queue property structs
        QueueFamilyProperties2 = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2,
        QueueFamilyGlobalPriorityPropertiesKHR = VK_STRUCTURE_TYPE_QUEUE_FAMILY_GLOBAL_PRIORITY_PROPERTIES_KHR,
        QueueFamilyCheckpointPropertiesNV = VK_STRUCTURE_TYPE_QUEUE_FAMILY_CHECKPOINT_PROPERTIES_NV,
        QueueFamilyCheckpointProperties2NV = VK_STRUCTURE_TYPE_QUEUE_FAMILY_CHECKPOINT_PROPERTIES_2_NV,

    #ifdef  _WIN32
        SurfaceCreateInfoWin32 = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
    #else
        SurfaceCreateInfoXlib = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
    #endif
    };
    
    template<>
    struct EnumVulkanConnect<StructureType> {
        using CorrespondingType = vk::StructureType;
    };

    template<>
    struct EnumVulkanConnect<vk::StructureType> {
        using CorrespondingType = StructureType;
    };

    template<>
    struct EnumCVulkanConnect<StructureType> {
        using CorrespondingType = VkStructureType;
    };

    template<>
    struct EnumCVulkanConnect<VkStructureType> {
        using CorrespondingType = StructureType;
    };


    enum class ColorSpace : uint32_t
    {
        SrgbNonlinear = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        DisplayP3NonlinearEXT = VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT,
        ExtendedSrgbLinearEXT = VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT,
        DisplayP3LinearEXT = VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT,
        DciP3NonlinearEXT = VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT,
        Bt709LinearEXT = VK_COLOR_SPACE_BT709_LINEAR_EXT,
        Bt709NonlinearEXT = VK_COLOR_SPACE_BT709_NONLINEAR_EXT,
        Bt2020LinearEXT = VK_COLOR_SPACE_BT2020_LINEAR_EXT,
        Hdr10St2084EXT = VK_COLOR_SPACE_HDR10_ST2084_EXT,
        Hdr10HlgEXT = VK_COLOR_SPACE_HDR10_HLG_EXT,
        AdobergbLinearEXT = VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT,
        AdobergbNonlinearEXT = VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT,
        PassThroughEXT = VK_COLOR_SPACE_PASS_THROUGH_EXT,
        ExtendedSrgbNonlinearEXT = VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT,
        DisplayNativeAMD = VK_COLOR_SPACE_DISPLAY_NATIVE_AMD
    };

    template<>
    struct EnumVulkanConnect<ColorSpace> {
        using CorrespondingType = vk::ColorSpaceKHR;
    };

    template<>
    struct EnumVulkanConnect<vk::ColorSpaceKHR> {
        using CorrespondingType = ColorSpace;
    };

    template<>
    struct EnumCVulkanConnect<ColorSpace> {
        using CorrespondingType = VkColorSpaceKHR;
    };

    template<>
    struct EnumCVulkanConnect<VkColorSpaceKHR> {
        using CorrespondingType = ColorSpace;
    };

    enum class ImageType : uint32_t
    {
        T1D = VK_IMAGE_TYPE_1D,
        T2D = VK_IMAGE_TYPE_2D,
        T3D = VK_IMAGE_TYPE_3D
    };

    template<>
    struct EnumVulkanConnect<ImageType> {
        using CorrespondingType = vk::ImageType;
    };

    template<>
    struct EnumVulkanConnect<vk::ImageType> {
        using CorrespondingType = ImageType;
    };

    template<>
    struct EnumCVulkanConnect<ImageType> {
        using CorrespondingType = VkImageType;
    };

    template<>
    struct EnumCVulkanConnect<VkImageType> {
        using CorrespondingType = ImageType;
    };

    enum class ImageViewType : uint32_t
    {
        T1D = VK_IMAGE_VIEW_TYPE_1D,
        T1DArray = VK_IMAGE_VIEW_TYPE_1D_ARRAY,
        T2D = VK_IMAGE_VIEW_TYPE_2D,
        T2DArray = VK_IMAGE_VIEW_TYPE_2D_ARRAY,
        T3D = VK_IMAGE_VIEW_TYPE_3D,
        Cube = VK_IMAGE_VIEW_TYPE_CUBE,
        CubeArray = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY,
    };

    template<>
    struct EnumVulkanConnect<ImageViewType> {
        using CorrespondingType = vk::ImageViewType;
    };

    template<>
    struct EnumVulkanConnect<vk::ImageViewType> {
        using CorrespondingType = ImageViewType;
    };

    template<>
    struct EnumCVulkanConnect<ImageViewType> {
        using CorrespondingType = VkImageViewType;
    };

    template<>
    struct EnumCVulkanConnect<VkImageViewType> {
        using CorrespondingType = ImageViewType;
    };

    enum class ComponentSwizzle : uint32_t
    {
        R = VK_COMPONENT_SWIZZLE_R,
        G = VK_COMPONENT_SWIZZLE_G,
        B = VK_COMPONENT_SWIZZLE_B,
        A = VK_COMPONENT_SWIZZLE_A,
        Identity = VK_COMPONENT_SWIZZLE_IDENTITY,
        Zero = VK_COMPONENT_SWIZZLE_ZERO,
        One = VK_COMPONENT_SWIZZLE_ONE,
    };

    template<>
    struct EnumVulkanConnect<ComponentSwizzle> {
        using CorrespondingType = vk::ComponentSwizzle;
    };

    template<>
    struct EnumVulkanConnect<vk::ComponentSwizzle> {
        using CorrespondingType = ComponentSwizzle;
    };

    template<>
    struct EnumCVulkanConnect<ComponentSwizzle> {
        using CorrespondingType = VkComponentSwizzle;
    };

    template<>
    struct EnumCVulkanConnect<VkComponentSwizzle> {
        using CorrespondingType = ComponentSwizzle;
    };

    enum class AttachmentLoadOp : uint32_t
    {
        None = VK_ATTACHMENT_LOAD_OP_NONE,
        NoneEXT = VK_ATTACHMENT_LOAD_OP_NONE_EXT,
        NoneKHR = VK_ATTACHMENT_LOAD_OP_NONE_KHR,
        DontCare = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        Load = VK_ATTACHMENT_LOAD_OP_LOAD,
        Clear = VK_ATTACHMENT_LOAD_OP_CLEAR
    };

    template<>
    struct EnumVulkanConnect<AttachmentLoadOp> {
        using CorrespondingType = vk::AttachmentLoadOp;
    };

    template<>
    struct EnumVulkanConnect<vk::AttachmentLoadOp> {
        using CorrespondingType = AttachmentLoadOp;
    };

    template<>
    struct EnumCVulkanConnect<AttachmentLoadOp> {
        using CorrespondingType = VkAttachmentLoadOp;
    };

    template<>
    struct EnumCVulkanConnect<VkAttachmentLoadOp> {
        using CorrespondingType = AttachmentLoadOp;
    };

    enum class AttachmentStoreOp : uint32_t
    {
        None = VK_ATTACHMENT_STORE_OP_NONE,
        NoneEXT = VK_ATTACHMENT_STORE_OP_NONE_EXT,
        NoneKHR = VK_ATTACHMENT_STORE_OP_NONE_KHR,
        NoneQCOM = VK_ATTACHMENT_STORE_OP_NONE_QCOM,
        DontCare = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        Store = VK_ATTACHMENT_STORE_OP_STORE,
    };

    template<>
    struct EnumVulkanConnect<AttachmentStoreOp> {
        using CorrespondingType = vk::AttachmentStoreOp;
    };

    template<>
    struct EnumVulkanConnect<vk::AttachmentStoreOp> {
        using CorrespondingType = AttachmentStoreOp;
    };

    template<>
    struct EnumCVulkanConnect<AttachmentStoreOp> {
        using CorrespondingType = VkAttachmentStoreOp;
    };

    template<>
    struct EnumCVulkanConnect<VkAttachmentStoreOp> {
        using CorrespondingType = AttachmentStoreOp;
    };

    enum class PipelineBindPoint : uint32_t
    {
        Graphics = VK_PIPELINE_BIND_POINT_GRAPHICS,
        Compute = VK_PIPELINE_BIND_POINT_COMPUTE,
        RayTracingKHR = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR,
        RayTracingNV = VK_PIPELINE_BIND_POINT_RAY_TRACING_NV,
        SubpassShadingHUAWEI = VK_PIPELINE_BIND_POINT_SUBPASS_SHADING_HUAWEI
    };

    template<>
    struct EnumVulkanConnect<PipelineBindPoint> {
        using CorrespondingType = vk::PipelineBindPoint;
    };

    template<>
    struct EnumVulkanConnect<vk::PipelineBindPoint> {
        using CorrespondingType = PipelineBindPoint;
    };

    template<>
    struct EnumCVulkanConnect<PipelineBindPoint> {
        using CorrespondingType = VkPipelineBindPoint;
    };

    template<>
    struct EnumCVulkanConnect<VkPipelineBindPoint> {
        using CorrespondingType = PipelineBindPoint;
    };

    enum class BorderColor : uint32_t
    {
        FloatCustomEXT = VK_BORDER_COLOR_FLOAT_CUSTOM_EXT,
        IntCustomEXT = VK_BORDER_COLOR_INT_CUSTOM_EXT,
        FloatTransparentBlack = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,
        IntTransparentBlack = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK,
        FloatOpaqueBlack = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
        IntOpaqueBlack = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        FloatOpaqueWhite = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
        IntOpaqueWhite = VK_BORDER_COLOR_INT_OPAQUE_WHITE,
    };

    template<>
    struct EnumVulkanConnect<BorderColor> {
        using CorrespondingType = vk::BorderColor;
    };

    template<>
    struct EnumVulkanConnect<vk::BorderColor> {
        using CorrespondingType = BorderColor;
    };

    template<>
    struct EnumCVulkanConnect<BorderColor> {
        using CorrespondingType = VkBorderColor;
    };

    template<>
    struct EnumCVulkanConnect<VkBorderColor> {
        using CorrespondingType = BorderColor;
    };

    enum class VertexInputRate : uint32_t
    {
        Instance = VK_VERTEX_INPUT_RATE_INSTANCE,
        Vertex = VK_VERTEX_INPUT_RATE_VERTEX,
    };

    template<>
    struct EnumVulkanConnect<VertexInputRate> {
        using CorrespondingType = vk::VertexInputRate;
    };

    template<>
    struct EnumVulkanConnect<vk::VertexInputRate> {
        using CorrespondingType = VertexInputRate;
    };

    template<>
    struct EnumCVulkanConnect<VertexInputRate> {
        using CorrespondingType = VkVertexInputRate;
    };

    template<>
    struct EnumCVulkanConnect<VkVertexInputRate> {
        using CorrespondingType = VertexInputRate;
    };

    enum class PrimitiveTopology : uint32_t
    {
        PointList = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
        LineList = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
        LineStrip = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
        TriangleList = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        TriangleStrip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
        TriangleFan = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,
        LineListWithAdjacency = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,
        LineStripWithAdjacency = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,
        TriangleListWithAdjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,
        TriangleStripWithAdjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,
        PatchList = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST,
    };

    template<>
    struct EnumVulkanConnect<PrimitiveTopology> {
        using CorrespondingType = vk::PrimitiveTopology;
    };

    template<>
    struct EnumVulkanConnect<vk::PrimitiveTopology> {
        using CorrespondingType = PrimitiveTopology;
    };

    template<>
    struct EnumCVulkanConnect<PrimitiveTopology> {
        using CorrespondingType = VkPrimitiveTopology;
    };

    template<>
    struct EnumCVulkanConnect<VkPrimitiveTopology> {
        using CorrespondingType = PrimitiveTopology;
    };

    enum class PolygonMode : uint32_t
    {
        Fill = VK_POLYGON_MODE_FILL,
        Line = VK_POLYGON_MODE_LINE,
        Point = VK_POLYGON_MODE_POINT,
        FillRectangleNV = VK_POLYGON_MODE_FILL_RECTANGLE_NV
    };

    template<>
    struct EnumVulkanConnect<PolygonMode> {
        using CorrespondingType = vk::PolygonMode;
    };

    template<>
    struct EnumVulkanConnect<vk::PolygonMode> {
        using CorrespondingType = PolygonMode;
    };

    template<>
    struct EnumCVulkanConnect<PolygonMode> {
        using CorrespondingType = VkPolygonMode;
    };

    template<>
    struct EnumCVulkanConnect<VkPolygonMode> {
        using CorrespondingType = PolygonMode;
    };

    enum class FrontFace : uint32_t
    {
        CounterClockwise = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        Clockwise = VK_FRONT_FACE_CLOCKWISE,
    };

    template<>
    struct EnumVulkanConnect<FrontFace> {
        using CorrespondingType = vk::FrontFace;
    };

    template<>
    struct EnumVulkanConnect<vk::FrontFace> {
        using CorrespondingType = FrontFace;
    };

    template<>
    struct EnumCVulkanConnect<FrontFace> {
        using CorrespondingType = VkFrontFace;
    };

    template<>
    struct EnumCVulkanConnect<VkFrontFace> {
        using CorrespondingType = FrontFace;
    };

    enum class CompareOp : uint32_t
    {
        Always = VK_COMPARE_OP_ALWAYS,
        Equal = VK_COMPARE_OP_EQUAL,
        Greater = VK_COMPARE_OP_GREATER,
        GreaterOrEqual = VK_COMPARE_OP_GREATER_OR_EQUAL,
        Less = VK_COMPARE_OP_LESS,
        LessOrEqual = VK_COMPARE_OP_LESS_OR_EQUAL,
        Never = VK_COMPARE_OP_NEVER,
        NotEqual = VK_COMPARE_OP_NOT_EQUAL
    };

    template<>
    struct EnumVulkanConnect<CompareOp> {
        using CorrespondingType = vk::CompareOp;
    };

    template<>
    struct EnumVulkanConnect<vk::CompareOp> {
        using CorrespondingType = CompareOp;
    };

    template<>
    struct EnumCVulkanConnect<CompareOp> {
        using CorrespondingType = VkCompareOp;
    };

    template<>
    struct EnumCVulkanConnect<VkCompareOp> {
        using CorrespondingType = CompareOp;
    };

    enum class StencilOp : uint32_t
    {
        DecrementAndClamp = VK_STENCIL_OP_DECREMENT_AND_CLAMP,
        DecrementAndWrap = VK_STENCIL_OP_DECREMENT_AND_WRAP,
        Invert = VK_STENCIL_OP_INVERT,
        IncrementAndClamp = VK_STENCIL_OP_INCREMENT_AND_CLAMP,
        IncrementAndWrap = VK_STENCIL_OP_INCREMENT_AND_WRAP,
        Keep = VK_STENCIL_OP_KEEP,
        Replace = VK_STENCIL_OP_REPLACE,
        Zero = VK_STENCIL_OP_ZERO
    };

    template<>
    struct EnumVulkanConnect<StencilOp> {
        using CorrespondingType = vk::StencilOp;
    };

    template<>
    struct EnumVulkanConnect<vk::StencilOp> {
        using CorrespondingType = StencilOp;
    };

    template<>
    struct EnumCVulkanConnect<StencilOp> {
        using CorrespondingType = VkStencilOp;
    };

    template<>
    struct EnumCVulkanConnect<VkStencilOp> {
        using CorrespondingType = StencilOp;
    };

    enum class LogicOp : uint32_t
    {
        And = VK_LOGIC_OP_AND,
        AndInverted = VK_LOGIC_OP_AND_INVERTED,
        AndReverse = VK_LOGIC_OP_AND_REVERSE,
        Clear = VK_LOGIC_OP_CLEAR,
        Copy = VK_LOGIC_OP_COPY,
        CopyInverted = VK_LOGIC_OP_COPY_INVERTED,
        Nand = VK_LOGIC_OP_NAND,
        NoOp = VK_LOGIC_OP_NO_OP,
        Nor = VK_LOGIC_OP_NOR,
        Or = VK_LOGIC_OP_OR,
        OrInverted = VK_LOGIC_OP_OR_INVERTED,
        OrReverse = VK_LOGIC_OP_OR_REVERSE,
        Set = VK_LOGIC_OP_SET,
        Xor = VK_LOGIC_OP_XOR,
        Invert = VK_LOGIC_OP_INVERT,
        Equivalent = VK_LOGIC_OP_EQUIVALENT,
    };

    template<>
    struct EnumVulkanConnect<LogicOp> {
        using CorrespondingType = vk::LogicOp;
    };

    template<>
    struct EnumVulkanConnect<vk::LogicOp> {
        using CorrespondingType = LogicOp;
    };

    template<>
    struct EnumCVulkanConnect<LogicOp> {
        using CorrespondingType = VkLogicOp;
    };

    template<>
    struct EnumCVulkanConnect<VkLogicOp> {
        using CorrespondingType = LogicOp;
    };

    enum class BlendFactor : uint32_t
    {
        Zero                  = VK_BLEND_FACTOR_ZERO,
        One                   = VK_BLEND_FACTOR_ONE,
        SrcColor              = VK_BLEND_FACTOR_SRC_COLOR,
        OneMinusSrcColor      = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
        DstColor              = VK_BLEND_FACTOR_DST_COLOR,
        OneMinusDstColor      = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
        SrcAlpha              = VK_BLEND_FACTOR_SRC_ALPHA,
        OneMinusSrcAlpha      = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        DstAlpha              = VK_BLEND_FACTOR_DST_ALPHA,
        OneMinusDstAlpha      = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
        ConstantColor         = VK_BLEND_FACTOR_CONSTANT_COLOR,
        OneMinusConstantColor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
        ConstantAlpha         = VK_BLEND_FACTOR_CONSTANT_ALPHA,
        OneMinusConstantAlpha = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
        SrcAlphaSaturate      = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
        Src1Color             = VK_BLEND_FACTOR_SRC1_COLOR,
        OneMinusSrc1Color     = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
        Src1Alpha             = VK_BLEND_FACTOR_SRC1_ALPHA,
        OneMinusSrc1Alpha     = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA
    };

    template<>
    struct EnumVulkanConnect<BlendFactor> {
        using CorrespondingType = vk::BlendFactor;
    };

    template<>
    struct EnumVulkanConnect<vk::BlendFactor> {
        using CorrespondingType = BlendFactor;
    };

    template<>
    struct EnumCVulkanConnect<BlendFactor> {
        using CorrespondingType = VkBlendFactor;
    };

    template<>
    struct EnumCVulkanConnect<VkBlendFactor> {
        using CorrespondingType = BlendFactor;
    };
    
    enum class BlendOp : uint32_t
    {
        Add                 = VK_BLEND_OP_ADD,
        Subtract            = VK_BLEND_OP_SUBTRACT,
        ReverseSubtract     = VK_BLEND_OP_REVERSE_SUBTRACT,
        Min                 = VK_BLEND_OP_MIN,
        Max                 = VK_BLEND_OP_MAX,
        ZeroEXT             = VK_BLEND_OP_ZERO_EXT,
        SrcEXT              = VK_BLEND_OP_SRC_EXT,
        DstEXT              = VK_BLEND_OP_DST_EXT,
        SrcOverEXT          = VK_BLEND_OP_SRC_OVER_EXT,
        DstOverEXT          = VK_BLEND_OP_DST_OVER_EXT,
        SrcInEXT            = VK_BLEND_OP_SRC_IN_EXT,
        DstInEXT            = VK_BLEND_OP_DST_IN_EXT,
        SrcOutEXT           = VK_BLEND_OP_SRC_OUT_EXT,
        DstOutEXT           = VK_BLEND_OP_DST_OUT_EXT,
        SrcAtopEXT          = VK_BLEND_OP_SRC_ATOP_EXT,
        DstAtopEXT          = VK_BLEND_OP_DST_ATOP_EXT,
        XorEXT              = VK_BLEND_OP_XOR_EXT,
        MultiplyEXT         = VK_BLEND_OP_MULTIPLY_EXT,
        ScreenEXT           = VK_BLEND_OP_SCREEN_EXT,
        OverlayEXT          = VK_BLEND_OP_OVERLAY_EXT,
        DarkenEXT           = VK_BLEND_OP_DARKEN_EXT,
        LightenEXT          = VK_BLEND_OP_LIGHTEN_EXT,
        ColordodgeEXT       = VK_BLEND_OP_COLORDODGE_EXT,
        ColorburnEXT        = VK_BLEND_OP_COLORBURN_EXT,
        HardlightEXT        = VK_BLEND_OP_HARDLIGHT_EXT,
        SoftlightEXT        = VK_BLEND_OP_SOFTLIGHT_EXT,
        DifferenceEXT       = VK_BLEND_OP_DIFFERENCE_EXT,
        ExclusionEXT        = VK_BLEND_OP_EXCLUSION_EXT,
        InvertEXT           = VK_BLEND_OP_INVERT_EXT,
        InvertRgbEXT        = VK_BLEND_OP_INVERT_RGB_EXT,
        LineardodgeEXT      = VK_BLEND_OP_LINEARDODGE_EXT,
        LinearburnEXT       = VK_BLEND_OP_LINEARBURN_EXT,
        VividlightEXT       = VK_BLEND_OP_VIVIDLIGHT_EXT,
        LinearlightEXT      = VK_BLEND_OP_LINEARLIGHT_EXT,
        PinlightEXT         = VK_BLEND_OP_PINLIGHT_EXT,
        HardmixEXT          = VK_BLEND_OP_HARDMIX_EXT,
        HslHueEXT           = VK_BLEND_OP_HSL_HUE_EXT,
        HslSaturationEXT    = VK_BLEND_OP_HSL_SATURATION_EXT,
        HslColorEXT         = VK_BLEND_OP_HSL_COLOR_EXT,
        HslLuminosityEXT    = VK_BLEND_OP_HSL_LUMINOSITY_EXT,
        PlusEXT             = VK_BLEND_OP_PLUS_EXT,
        PlusClampedEXT      = VK_BLEND_OP_PLUS_CLAMPED_EXT,
        PlusClampedAlphaEXT = VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT,
        PlusDarkerEXT       = VK_BLEND_OP_PLUS_DARKER_EXT,
        MinusEXT            = VK_BLEND_OP_MINUS_EXT,
        MinusClampedEXT     = VK_BLEND_OP_MINUS_CLAMPED_EXT,
        ContrastEXT         = VK_BLEND_OP_CONTRAST_EXT,
        InvertOvgEXT        = VK_BLEND_OP_INVERT_OVG_EXT,
        RedEXT              = VK_BLEND_OP_RED_EXT,
        GreenEXT            = VK_BLEND_OP_GREEN_EXT,
        BlueEXT             = VK_BLEND_OP_BLUE_EXT
    };

    template<>
    struct EnumVulkanConnect<BlendOp> {
        using CorrespondingType = vk::BlendOp;
    };

    template<>
    struct EnumVulkanConnect<vk::BlendOp> {
        using CorrespondingType = BlendOp;
    };

    template<>
    struct EnumCVulkanConnect<BlendOp> {
        using CorrespondingType = VkBlendOp;
    };

    template<>
    struct EnumCVulkanConnect<VkBlendOp> {
        using CorrespondingType = BlendOp;
    };

    enum class DynamicState : uint32_t
    {
        Viewport                            = VK_DYNAMIC_STATE_VIEWPORT,
        Scissor                             = VK_DYNAMIC_STATE_SCISSOR,
        LineWidth                           = VK_DYNAMIC_STATE_LINE_WIDTH,
        DepthBias                           = VK_DYNAMIC_STATE_DEPTH_BIAS,
        BlendConstants                      = VK_DYNAMIC_STATE_BLEND_CONSTANTS,
        DepthBounds                         = VK_DYNAMIC_STATE_DEPTH_BOUNDS,
        StencilCompareMask                  = VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
        StencilWriteMask                    = VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
        etencilReference                    = VK_DYNAMIC_STATE_STENCIL_REFERENCE,
        CullMode                            = VK_DYNAMIC_STATE_CULL_MODE,
        CullModeEXT                         = VK_DYNAMIC_STATE_CULL_MODE_EXT,
        FrontFace                           = VK_DYNAMIC_STATE_FRONT_FACE,
        FrontFaceEXT                        = VK_DYNAMIC_STATE_FRONT_FACE_EXT,
        PrimitiveTopology                   = VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY,
        PrimitiveTopologyEXT                = VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY_EXT,
        ViewportWithCount                   = VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT,
        ViewportWithCountEXT                = VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT_EXT,
        ScissorWithCount                    = VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT,
        ScissorWithCountEXT                 = VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT_EXT,
        VertexInputBindingStride            = VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE,
        VertexInputBindingStrideEXT         = VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE_EXT,
        DepthTestEnable                     = VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE,
        DepthTestEnableEXT                  = VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE_EXT,
        DepthWriteEnable                    = VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE,
        DepthWriteEnableEXT                 = VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE_EXT,
        DepthCompareOp                      = VK_DYNAMIC_STATE_DEPTH_COMPARE_OP,
        DepthCompareOpEXT                   = VK_DYNAMIC_STATE_DEPTH_COMPARE_OP_EXT,
        DepthBoundsTestEnable               = VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE,
        DepthBoundsTestEnableEXT            = VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE_EXT,
        StencilTestEnable                   = VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE,
        StencilTestEnableEXT                = VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE_EXT,
        StencilOp                           = VK_DYNAMIC_STATE_STENCIL_OP,
        StencilOpEXT                        = VK_DYNAMIC_STATE_STENCIL_OP_EXT,
        RasterizerDiscardEnable             = VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE,
        RasterizerDiscardEnableEXT          = VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE_EXT,
        DepthBiasEnable                     = VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE,
        DepthBiasEnableEXT                  = VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE_EXT,
        PrimitiveRestartEnable              = VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE,
        PrimitiveRestartEnableEXT           = VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE_EXT,
        LineStipple                         = VK_DYNAMIC_STATE_LINE_STIPPLE,
        LineStippleEXT                      = VK_DYNAMIC_STATE_LINE_STIPPLE_EXT,
        LineStippleKHR                      = VK_DYNAMIC_STATE_LINE_STIPPLE_KHR,
        ViewportWScalingNV                  = VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_NV,
        DiscardRectangleEXT                 = VK_DYNAMIC_STATE_DISCARD_RECTANGLE_EXT,
        DiscardRectangleEnableEXT           = VK_DYNAMIC_STATE_DISCARD_RECTANGLE_ENABLE_EXT,
        DiscardRectangleModeEXT             = VK_DYNAMIC_STATE_DISCARD_RECTANGLE_MODE_EXT,
        SampleLocationsEXT                  = VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT,
        RayTracingPipelineStackSizeKHR      = VK_DYNAMIC_STATE_RAY_TRACING_PIPELINE_STACK_SIZE_KHR,
        ViewportShadingRatePaletteNV        = VK_DYNAMIC_STATE_VIEWPORT_SHADING_RATE_PALETTE_NV,
        ViewportCoarseSampleOrderNV         = VK_DYNAMIC_STATE_VIEWPORT_COARSE_SAMPLE_ORDER_NV,
        ExclusiveScissorEnableNV            = VK_DYNAMIC_STATE_EXCLUSIVE_SCISSOR_ENABLE_NV,
        ExclusiveScissorNV                  = VK_DYNAMIC_STATE_EXCLUSIVE_SCISSOR_NV,
        FragmentShadingRateKHR              = VK_DYNAMIC_STATE_FRAGMENT_SHADING_RATE_KHR,
        VertexInputEXT                      = VK_DYNAMIC_STATE_VERTEX_INPUT_EXT,
        PatchControlPointsEXT               = VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT,
        LogicOpEXT                          = VK_DYNAMIC_STATE_LOGIC_OP_EXT,
        ColorWriteEnableEXT                 = VK_DYNAMIC_STATE_COLOR_WRITE_ENABLE_EXT,
        DepthClampEnableEXT                 = VK_DYNAMIC_STATE_DEPTH_CLAMP_ENABLE_EXT,
        PolygonModeEXT                      = VK_DYNAMIC_STATE_POLYGON_MODE_EXT,
        RasterizationSamplesEXT             = VK_DYNAMIC_STATE_RASTERIZATION_SAMPLES_EXT,
        SampleMaskEXT                       = VK_DYNAMIC_STATE_SAMPLE_MASK_EXT,
        AlphaToCoverageEnableEXT            = VK_DYNAMIC_STATE_ALPHA_TO_COVERAGE_ENABLE_EXT,
        AlphaToOneEnableEXT                 = VK_DYNAMIC_STATE_ALPHA_TO_ONE_ENABLE_EXT,
        LogicOpEnableEXT                    = VK_DYNAMIC_STATE_LOGIC_OP_ENABLE_EXT,
        ColorBlendEnableEXT                 = VK_DYNAMIC_STATE_COLOR_BLEND_ENABLE_EXT,
        ColorBlendEquationEXT               = VK_DYNAMIC_STATE_COLOR_BLEND_EQUATION_EXT,
        ColorWriteMaskEXT                   = VK_DYNAMIC_STATE_COLOR_WRITE_MASK_EXT,
        TessellationDomainOriginEXT         = VK_DYNAMIC_STATE_TESSELLATION_DOMAIN_ORIGIN_EXT,
        RasterizationStreamEXT              = VK_DYNAMIC_STATE_RASTERIZATION_STREAM_EXT,
        ConservativeRasterizationModeEXT    = VK_DYNAMIC_STATE_CONSERVATIVE_RASTERIZATION_MODE_EXT,
        ExtraPrimitiveOverestimationSizeEXT = VK_DYNAMIC_STATE_EXTRA_PRIMITIVE_OVERESTIMATION_SIZE_EXT,
        DepthClipEnableEXT                  = VK_DYNAMIC_STATE_DEPTH_CLIP_ENABLE_EXT,
        SampleLocationsEnableEXT            = VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_ENABLE_EXT,
        ColorBlendAdvancedEXT               = VK_DYNAMIC_STATE_COLOR_BLEND_ADVANCED_EXT,
        ProvokingVertexModeEXT              = VK_DYNAMIC_STATE_PROVOKING_VERTEX_MODE_EXT,
        LineRasterizationModeEXT            = VK_DYNAMIC_STATE_LINE_RASTERIZATION_MODE_EXT,
        LineStippleEnableEXT                = VK_DYNAMIC_STATE_LINE_STIPPLE_ENABLE_EXT,
        DepthClipNegativeOneToOneEXT        = VK_DYNAMIC_STATE_DEPTH_CLIP_NEGATIVE_ONE_TO_ONE_EXT,
        ViewportWScalingEnableNV            = VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_ENABLE_NV,
        ViewportSwizzleNV                   = VK_DYNAMIC_STATE_VIEWPORT_SWIZZLE_NV,
        CoverageToColorEnableNV             = VK_DYNAMIC_STATE_COVERAGE_TO_COLOR_ENABLE_NV,
        CoverageToColorLocationNV           = VK_DYNAMIC_STATE_COVERAGE_TO_COLOR_LOCATION_NV,
        CoverageModulationModeNV            = VK_DYNAMIC_STATE_COVERAGE_MODULATION_MODE_NV,
        CoverageModulationTableEnableNV     = VK_DYNAMIC_STATE_COVERAGE_MODULATION_TABLE_ENABLE_NV,
        CoverageModulationTableNV           = VK_DYNAMIC_STATE_COVERAGE_MODULATION_TABLE_NV,
        ShadingRateImageEnableNV            = VK_DYNAMIC_STATE_SHADING_RATE_IMAGE_ENABLE_NV,
        RepresentativeFragmentTestEnableNV  = VK_DYNAMIC_STATE_REPRESENTATIVE_FRAGMENT_TEST_ENABLE_NV,
        CoverageReductionModeNV             = VK_DYNAMIC_STATE_COVERAGE_REDUCTION_MODE_NV,
        AttachmentFeedbackLoopEnableEXT     = VK_DYNAMIC_STATE_ATTACHMENT_FEEDBACK_LOOP_ENABLE_EXT,
        DepthClampRangeEXT                  = VK_DYNAMIC_STATE_DEPTH_CLAMP_RANGE_EXT
    };

    template<>
    struct EnumVulkanConnect<DynamicState> {
        using CorrespondingType = vk::DynamicState;
    };

    template<>
    struct EnumVulkanConnect<vk::DynamicState> {
        using CorrespondingType = DynamicState;
    };

    template<>
    struct EnumCVulkanConnect<DynamicState> {
        using CorrespondingType = VkDynamicState;
    };

    template<>
    struct EnumCVulkanConnect<VkDynamicState> {
        using CorrespondingType = DynamicState;
    };

    enum class SubpassContents : uint32_t
    {
        Inline                              = VK_SUBPASS_CONTENTS_INLINE,
        SecondaryCommandBuffers             = VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS,
        InlineAndSecondaryCommandBuffersKHR = VK_SUBPASS_CONTENTS_INLINE_AND_SECONDARY_COMMAND_BUFFERS_KHR,
        InlineAndSecondaryCommandBuffersEXT = VK_SUBPASS_CONTENTS_INLINE_AND_SECONDARY_COMMAND_BUFFERS_EXT
    };

    template<>
    struct EnumVulkanConnect<SubpassContents> {
        using CorrespondingType = vk::SubpassContents;
    };

    template<>
    struct EnumVulkanConnect<vk::SubpassContents> {
        using CorrespondingType = SubpassContents;
    };

    template<>
    struct EnumCVulkanConnect<SubpassContents> {
        using CorrespondingType = VkSubpassContents;
    };

    template<>
    struct EnumCVulkanConnect<VkSubpassContents> {
        using CorrespondingType = SubpassContents;
    };    

    enum class IndexType : uint32_t
    {
        Uint16   = VK_INDEX_TYPE_UINT16,
        Uint32   = VK_INDEX_TYPE_UINT32,
        Uint8    = VK_INDEX_TYPE_UINT8,
        Uint8EXT = VK_INDEX_TYPE_UINT8_EXT,
        Uint8KHR = VK_INDEX_TYPE_UINT8_KHR,
        NoneKHR  = VK_INDEX_TYPE_NONE_KHR,
        NoneNV   = VK_INDEX_TYPE_NONE_NV
    };

    template<>
    struct EnumVulkanConnect<IndexType> {
        using CorrespondingType = vk::IndexType;
    };

    template<>
    struct EnumVulkanConnect<vk::IndexType> {
        using CorrespondingType = IndexType;
    };

    template<>
    struct EnumCVulkanConnect<IndexType> {
        using CorrespondingType = VkIndexType;
    };

    template<>
    struct EnumCVulkanConnect<VkIndexType> {
        using CorrespondingType = IndexType;
    };

    enum class InternalAllocationType : uint32_t {
        Executable = VK_INTERNAL_ALLOCATION_TYPE_EXECUTABLE
    };

    template<>
    struct EnumVulkanConnect<InternalAllocationType> {
        using CorrespondingType = vk::InternalAllocationType;
    };

    template<>
    struct EnumVulkanConnect<vk::InternalAllocationType> {
        using CorrespondingType = InternalAllocationType;
    };

    template<>
    struct EnumCVulkanConnect<InternalAllocationType> {
        using CorrespondingType = VkInternalAllocationType;
    };

    template<>
    struct EnumCVulkanConnect<VkInternalAllocationType> {
        using CorrespondingType = InternalAllocationType;
    };

    enum class SystemAllocationScope : uint32_t {
        Command  = VK_SYSTEM_ALLOCATION_SCOPE_COMMAND,
        Object   = VK_SYSTEM_ALLOCATION_SCOPE_OBJECT,
        Cache    = VK_SYSTEM_ALLOCATION_SCOPE_CACHE,
        Device   = VK_SYSTEM_ALLOCATION_SCOPE_DEVICE,
        Instance = VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE
    };

    template<>
    struct EnumVulkanConnect<SystemAllocationScope> {
        using CorrespondingType = vk::SystemAllocationScope;
    };

    template<>
    struct EnumVulkanConnect<vk::SystemAllocationScope> {
        using CorrespondingType = SystemAllocationScope;
    };

    template<>
    struct EnumCVulkanConnect<SystemAllocationScope> {
        using CorrespondingType = VkSystemAllocationScope;
    };

    template<>
    struct EnumCVulkanConnect<VkSystemAllocationScope> {
        using CorrespondingType = SystemAllocationScope;
    };

    //converts enums to and from vulkan
    template<typename Enum>
    constexpr auto convertEnum(const Enum& val) { 
        return static_cast<typename EnumVulkanConnect<Enum>::CorrespondingType>(val); 
    };

    template<typename Enum>
    constexpr auto convertCEnum(const Enum& val) {
        return static_cast<typename EnumCVulkanConnect<Enum>::CorrespondingType>(val);
    };

    template<typename Enum>
    constexpr auto convertEnum(const Enum* val) {
        return reinterpret_cast<const typename EnumVulkanConnect<Enum>::CorrespondingType*>(val);
    };

    template<typename Enum>
    constexpr auto convertCEnum(const Enum* val) {
        return reinterpret_cast<const typename EnumCVulkanConnect<Enum>::CorrespondingType*>(val);
    };

    template<typename Enum>
    constexpr auto enumToInt(const Enum& val) { return static_cast<std::underlying_type_t<Enum>>(val); };
}