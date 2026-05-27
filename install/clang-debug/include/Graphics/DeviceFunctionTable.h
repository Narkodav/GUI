#pragma once
#include "FunctionTable.h"

#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

#include "Enums.h"

namespace Graphics {

    template <DeviceFunction function>
    struct DeviceFunctionTraits {};

    // Core device functions
    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyDevice> {
        using Type = PFN_vkDestroyDevice;
        static constexpr const char* name = "vkDestroyDevice";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetDeviceQueue> {
        using Type = PFN_vkGetDeviceQueue;
        static constexpr const char* name = "vkGetDeviceQueue";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetDeviceQueue2> {
        using Type = PFN_vkGetDeviceQueue2;
        static constexpr const char* name = "vkGetDeviceQueue2";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DeviceWaitIdle> {
        using Type = PFN_vkDeviceWaitIdle;
        static constexpr const char* name = "vkDeviceWaitIdle";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::AllocateMemory> {
        using Type = PFN_vkAllocateMemory;
        static constexpr const char* name = "vkAllocateMemory";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::FreeMemory> {
        using Type = PFN_vkFreeMemory;
        static constexpr const char* name = "vkFreeMemory";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::MapMemory> {
        using Type = PFN_vkMapMemory;
        static constexpr const char* name = "vkMapMemory";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::UnmapMemory> {
        using Type = PFN_vkUnmapMemory;
        static constexpr const char* name = "vkUnmapMemory";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::FlushMappedMemoryRanges> {
        using Type = PFN_vkFlushMappedMemoryRanges;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkFlushMappedMemoryRanges";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::InvalidateMappedMemoryRanges> {
        using Type = PFN_vkInvalidateMappedMemoryRanges;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkInvalidateMappedMemoryRanges";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetDeviceMemoryCommitment> {
        using Type = PFN_vkGetDeviceMemoryCommitment;
        using ReturnType = void;
        static constexpr const char* name = "vkGetDeviceMemoryCommitment";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::BindBufferMemory> {
        using Type = PFN_vkBindBufferMemory;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkBindBufferMemory";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::BindImageMemory> {
        using Type = PFN_vkBindImageMemory;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkBindImageMemory";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetBufferMemoryRequirements> {
        using Type = PFN_vkGetBufferMemoryRequirements;
        using ReturnType = void;
        static constexpr const char* name = "vkGetBufferMemoryRequirements";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetImageMemoryRequirements> {
        using Type = PFN_vkGetImageMemoryRequirements;
        using ReturnType = void;
        static constexpr const char* name = "vkGetImageMemoryRequirements";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetImageSparseMemoryRequirements> {
        using Type = PFN_vkGetImageSparseMemoryRequirements;
        using ReturnType = void;
        static constexpr const char* name = "vkGetImageSparseMemoryRequirements";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::QueueSubmit> {
        using Type = PFN_vkQueueSubmit;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkQueueSubmit";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::QueueWaitIdle> {
        using Type = PFN_vkQueueWaitIdle;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkQueueWaitIdle";
    };

    // Command buffer functions
    template <>
    struct DeviceFunctionTraits<DeviceFunction::AllocateCommandBuffers> {
        using Type = PFN_vkAllocateCommandBuffers;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkAllocateCommandBuffers";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::FreeCommandBuffers> {
        using Type = PFN_vkFreeCommandBuffers;
        using ReturnType = void;
        static constexpr const char* name = "vkFreeCommandBuffers";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::BeginCommandBuffer> {
        using Type = PFN_vkBeginCommandBuffer;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkBeginCommandBuffer";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::EndCommandBuffer> {
        using Type = PFN_vkEndCommandBuffer;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkEndCommandBuffer";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::ResetCommandBuffer> {
        using Type = PFN_vkResetCommandBuffer;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkResetCommandBuffer";
    };

    // Command functions (just a few examples - you'd continue this pattern)
    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdBindPipeline> {
        using Type = PFN_vkCmdBindPipeline;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdBindPipeline";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdSetViewport> {
        using Type = PFN_vkCmdSetViewport;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdSetViewport";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdSetScissor> {
        using Type = PFN_vkCmdSetScissor;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdSetScissor";
    };

    // ... Continue this pattern for all Cmd* functions

    // VK_KHR_swapchain extension
    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateSwapchainKHR> {
        using Type = PFN_vkCreateSwapchainKHR;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkCreateSwapchainKHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroySwapchainKHR> {
        using Type = PFN_vkDestroySwapchainKHR;
        using ReturnType = void;
        static constexpr const char* name = "vkDestroySwapchainKHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetSwapchainImagesKHR> {
        using Type = PFN_vkGetSwapchainImagesKHR;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkGetSwapchainImagesKHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::AcquireNextImageKHR> {
        using Type = PFN_vkAcquireNextImageKHR;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkAcquireNextImageKHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::QueuePresentKHR> {
        using Type = PFN_vkQueuePresentKHR;
        using ReturnType = VkResult;
        static constexpr const char* name = "vkQueuePresentKHR";
    };

    // VK_EXT_debug_utils extension (device functions)
    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdBeginDebugUtilsLabelEXT> {
        using Type = PFN_vkCmdBeginDebugUtilsLabelEXT;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdBeginDebugUtilsLabelEXT";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdEndDebugUtilsLabelEXT> {
        using Type = PFN_vkCmdEndDebugUtilsLabelEXT;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdEndDebugUtilsLabelEXT";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdInsertDebugUtilsLabelEXT> {
        using Type = PFN_vkCmdInsertDebugUtilsLabelEXT;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdInsertDebugUtilsLabelEXT";
    };

    // Missing core command functions
    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdSetLineWidth> {
        using Type = PFN_vkCmdSetLineWidth;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdSetLineWidth";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdSetDepthBias> {
        using Type = PFN_vkCmdSetDepthBias;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdSetDepthBias";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdSetBlendConstants> {
        using Type = PFN_vkCmdSetBlendConstants;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdSetBlendConstants";
    };
    
    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdSetDepthBounds> {
        using Type = PFN_vkCmdSetDepthBounds;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdSetDepthBounds";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdSetStencilCompareMask> {
        using Type = PFN_vkCmdSetStencilCompareMask;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdSetStencilCompareMask";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdSetStencilWriteMask> {
        using Type = PFN_vkCmdSetStencilWriteMask;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdSetStencilWriteMask";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdSetStencilReference> {
        using Type = PFN_vkCmdSetStencilReference;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdSetStencilReference";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdBindDescriptorSets> {
        using Type = PFN_vkCmdBindDescriptorSets;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdBindDescriptorSets";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdBindIndexBuffer> {
        using Type = PFN_vkCmdBindIndexBuffer;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdBindIndexBuffer";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdBindVertexBuffers> {
        using Type = PFN_vkCmdBindVertexBuffers;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdBindVertexBuffers";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdDraw> {
        using Type = PFN_vkCmdDraw;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdDraw";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdDrawIndexed> {
        using Type = PFN_vkCmdDrawIndexed;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdDrawIndexed";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdDrawIndirect> {
        using Type = PFN_vkCmdDrawIndirect;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdDrawIndirect";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdDrawIndexedIndirect> {
        using Type = PFN_vkCmdDrawIndexedIndirect;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdDrawIndexedIndirect";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdDispatch> {
        using Type = PFN_vkCmdDispatch;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdDispatch";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdDispatchIndirect> {
        using Type = PFN_vkCmdDispatchIndirect;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdDispatchIndirect";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdCopyBuffer> {
        using Type = PFN_vkCmdCopyBuffer;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdCopyBuffer";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdCopyImage> {
        using Type = PFN_vkCmdCopyImage;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdCopyImage";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdBlitImage> {
        using Type = PFN_vkCmdBlitImage;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdBlitImage";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdCopyBufferToImage> {
        using Type = PFN_vkCmdCopyBufferToImage;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdCopyBufferToImage";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdCopyImageToBuffer> {
        using Type = PFN_vkCmdCopyImageToBuffer;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdCopyImageToBuffer";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdUpdateBuffer> {
        using Type = PFN_vkCmdUpdateBuffer;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdUpdateBuffer";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdFillBuffer> {
        using Type = PFN_vkCmdFillBuffer;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdFillBuffer";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdClearColorImage> {
        using Type = PFN_vkCmdClearColorImage;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdClearColorImage";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdClearDepthStencilImage> {
        using Type = PFN_vkCmdClearDepthStencilImage;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdClearDepthStencilImage";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdClearAttachments> {
        using Type = PFN_vkCmdClearAttachments;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdClearAttachments";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdResolveImage> {
        using Type = PFN_vkCmdResolveImage;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdResolveImage";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdSetEvent> {
        using Type = PFN_vkCmdSetEvent;
        using ReturnType = void;
        static constexpr const char* name = "vkCmdSetEvent";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdResetEvent> {
        using Type = PFN_vkCmdResetEvent;
        static constexpr const char* name = "vkCmdResetEvent";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdWaitEvents> {
        using Type = PFN_vkCmdWaitEvents;
        static constexpr const char* name = "vkCmdWaitEvents";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdPipelineBarrier> {
        using Type = PFN_vkCmdPipelineBarrier;
        static constexpr const char* name = "vkCmdPipelineBarrier";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdBeginQuery> {
        using Type = PFN_vkCmdBeginQuery;
        static constexpr const char* name = "vkCmdBeginQuery";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdEndQuery> {
        using Type = PFN_vkCmdEndQuery;
        static constexpr const char* name = "vkCmdEndQuery";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdResetQueryPool> {
        using Type = PFN_vkCmdResetQueryPool;
        static constexpr const char* name = "vkCmdResetQueryPool";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdWriteTimestamp> {
        using Type = PFN_vkCmdWriteTimestamp;
        static constexpr const char* name = "vkCmdWriteTimestamp";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdCopyQueryPoolResults> {
        using Type = PFN_vkCmdCopyQueryPoolResults;
        static constexpr const char* name = "vkCmdCopyQueryPoolResults";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdPushConstants> {
        using Type = PFN_vkCmdPushConstants;
        static constexpr const char* name = "vkCmdPushConstants";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdBeginRenderPass> {
        using Type = PFN_vkCmdBeginRenderPass;
        static constexpr const char* name = "vkCmdBeginRenderPass";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdNextSubpass> {
        using Type = PFN_vkCmdNextSubpass;
        static constexpr const char* name = "vkCmdNextSubpass";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdEndRenderPass> {
        using Type = PFN_vkCmdEndRenderPass;
        static constexpr const char* name = "vkCmdEndRenderPass";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdExecuteCommands> {
        using Type = PFN_vkCmdExecuteCommands;
        static constexpr const char* name = "vkCmdExecuteCommands";
    };

    // Missing VK_EXT_debug_utils queue functions
    template <>
    struct DeviceFunctionTraits<DeviceFunction::QueueBeginDebugUtilsLabelEXT> {
        using Type = PFN_vkQueueBeginDebugUtilsLabelEXT;
        static constexpr const char* name = "vkQueueBeginDebugUtilsLabelEXT";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::QueueEndDebugUtilsLabelEXT> {
        using Type = PFN_vkQueueEndDebugUtilsLabelEXT;
        static constexpr const char* name = "vkQueueEndDebugUtilsLabelEXT";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::QueueInsertDebugUtilsLabelEXT> {
        using Type = PFN_vkQueueInsertDebugUtilsLabelEXT;
        static constexpr const char* name = "vkQueueInsertDebugUtilsLabelEXT";
    };

    // VK_KHR_synchronization2 extension functions
    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdSetEvent2KHR> {
        using Type = PFN_vkCmdSetEvent2KHR;
        static constexpr const char* name = "vkCmdSetEvent2KHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdResetEvent2KHR> {
        using Type = PFN_vkCmdResetEvent2KHR;
        static constexpr const char* name = "vkCmdResetEvent2KHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdWaitEvents2KHR> {
        using Type = PFN_vkCmdWaitEvents2KHR;
        static constexpr const char* name = "vkCmdWaitEvents2KHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdPipelineBarrier2KHR> {
        using Type = PFN_vkCmdPipelineBarrier2KHR;
        static constexpr const char* name = "vkCmdPipelineBarrier2KHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdWriteTimestamp2KHR> {
        using Type = PFN_vkCmdWriteTimestamp2KHR;
        static constexpr const char* name = "vkCmdWriteTimestamp2KHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::QueueSubmit2KHR> {
        using Type = PFN_vkQueueSubmit2KHR;
        static constexpr const char* name = "vkQueueSubmit2KHR";
    };

    // VK_KHR_buffer_device_address extension functions
    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetBufferDeviceAddressKHR> {
        using Type = PFN_vkGetBufferDeviceAddressKHR;
        static constexpr const char* name = "vkGetBufferDeviceAddressKHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetBufferOpaqueCaptureAddressKHR> {
        using Type = PFN_vkGetBufferOpaqueCaptureAddressKHR;
        static constexpr const char* name = "vkGetBufferOpaqueCaptureAddressKHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetDeviceMemoryOpaqueCaptureAddressKHR> {
        using Type = PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR;
        static constexpr const char* name = "vkGetDeviceMemoryOpaqueCaptureAddressKHR";
    };

    // VK_KHR_dynamic_rendering extension functions
    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdBeginRenderingKHR> {
        using Type = PFN_vkCmdBeginRenderingKHR;
        static constexpr const char* name = "vkCmdBeginRenderingKHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdEndRenderingKHR> {
        using Type = PFN_vkCmdEndRenderingKHR;
        static constexpr const char* name = "vkCmdEndRenderingKHR";
    };

    // VK_KHR_timeline_semaphore extension functions
    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetSemaphoreCounterValueKHR> {
        using Type = PFN_vkGetSemaphoreCounterValueKHR;
        static constexpr const char* name = "vkGetSemaphoreCounterValueKHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::WaitSemaphoresKHR> {
        using Type = PFN_vkWaitSemaphoresKHR;
        static constexpr const char* name = "vkWaitSemaphoresKHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::SignalSemaphoreKHR> {
        using Type = PFN_vkSignalSemaphoreKHR;
        static constexpr const char* name = "vkSignalSemaphoreKHR";
    };

    // VK_KHR_copy_commands2 extension functions
    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdCopyBuffer2KHR> {
        using Type = PFN_vkCmdCopyBuffer2KHR;
        static constexpr const char* name = "vkCmdCopyBuffer2KHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdCopyImage2KHR> {
        using Type = PFN_vkCmdCopyImage2KHR;
        static constexpr const char* name = "vkCmdCopyImage2KHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdCopyBufferToImage2KHR> {
        using Type = PFN_vkCmdCopyBufferToImage2KHR;
        static constexpr const char* name = "vkCmdCopyBufferToImage2KHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdCopyImageToBuffer2KHR> {
        using Type = PFN_vkCmdCopyImageToBuffer2KHR;
        static constexpr const char* name = "vkCmdCopyImageToBuffer2KHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdBlitImage2KHR> {
        using Type = PFN_vkCmdBlitImage2KHR;
        static constexpr const char* name = "vkCmdBlitImage2KHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CmdResolveImage2KHR> {
        using Type = PFN_vkCmdResolveImage2KHR;
        static constexpr const char* name = "vkCmdResolveImage2KHR";
    };

    // VK_KHR_maintenance4 extension functions
    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetDeviceBufferMemoryRequirementsKHR> {
        using Type = PFN_vkGetDeviceBufferMemoryRequirementsKHR;
        static constexpr const char* name = "vkGetDeviceBufferMemoryRequirementsKHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetDeviceImageMemoryRequirementsKHR> {
        using Type = PFN_vkGetDeviceImageMemoryRequirementsKHR;
        static constexpr const char* name = "vkGetDeviceImageMemoryRequirementsKHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::GetDeviceImageSparseMemoryRequirementsKHR> {
        using Type = PFN_vkGetDeviceImageSparseMemoryRequirementsKHR;
        static constexpr const char* name = "vkGetDeviceImageSparseMemoryRequirementsKHR";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyRenderPass> {
        using Type = PFN_vkDestroyRenderPass;
        static constexpr const char* name = "vkDestroyRenderPass";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateRenderPass> {
        using Type = PFN_vkCreateRenderPass;
        static constexpr const char* name = "vkCreateRenderPass";
    };    

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyBuffer> {
        using Type = PFN_vkDestroyBuffer;
        static constexpr const char* name = "vkDestroyBuffer";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateBuffer> {
        using Type = PFN_vkCreateBuffer;
        static constexpr const char* name = "vkCreateBuffer";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyImage> {
        using Type = PFN_vkDestroyImage;
        static constexpr const char* name = "vkDestroyImage";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyImageView> {
        using Type = PFN_vkDestroyImageView;
        static constexpr const char* name = "vkDestroyImageView";
    };
    
    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateImage> {
        using Type = PFN_vkCreateImage;
        static constexpr const char* name = "vkCreateImage";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateImageView> {
        using Type = PFN_vkCreateImageView;
        static constexpr const char* name = "vkCreateImageView";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroySemaphore> {
        using Type = PFN_vkDestroySemaphore;
        static constexpr const char* name = "vkDestroySemaphore";
    };
    
    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateSemaphore> {
        using Type = PFN_vkCreateSemaphore;
        static constexpr const char* name = "vkCreateSemaphore";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyFence> {
        using Type = PFN_vkDestroyFence;
        static constexpr const char* name = "vkDestroyFence";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateFence> {
        using Type = PFN_vkCreateFence;
        static constexpr const char* name = "vkCreateFence";
    };
    
    template <>
    struct DeviceFunctionTraits<DeviceFunction::WaitForFences> {
        using Type = PFN_vkWaitForFences;
        static constexpr const char* name = "vkWaitForFences";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::ResetFences> {
        using Type = PFN_vkResetFences;
        static constexpr const char* name = "vkResetFences";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyShaderModule> {
        using Type = PFN_vkDestroyShaderModule;
        static constexpr const char* name = "vkDestroyShaderModule";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateShaderModule> {
        using Type = PFN_vkCreateShaderModule;
        static constexpr const char* name = "vkCreateShaderModule";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyFramebuffer> {
        using Type = PFN_vkDestroyFramebuffer;
        static constexpr const char* name = "vkDestroyFramebuffer";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateFramebuffer> {
        using Type = PFN_vkCreateFramebuffer;
        static constexpr const char* name = "vkCreateFramebuffer";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyCommandPool> {
        using Type = PFN_vkDestroyCommandPool;
        static constexpr const char* name = "vkDestroyCommandPool";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateCommandPool> {
        using Type = PFN_vkCreateCommandPool;
        static constexpr const char* name = "vkCreateCommandPool";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::ResetCommandPool> {
        using Type = PFN_vkResetCommandPool;
        static constexpr const char* name = "vkResetCommandPool";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateSampler> {
        using Type = PFN_vkCreateSampler;
        static constexpr const char* name = "vkCreateSampler";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroySampler> {
        using Type = PFN_vkDestroySampler;
        static constexpr const char* name = "vkDestroySampler";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateDescriptorSetLayout> {
        using Type = PFN_vkCreateDescriptorSetLayout;
        static constexpr const char* name = "vkCreateDescriptorSetLayout";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyDescriptorSetLayout> {
        using Type = PFN_vkDestroyDescriptorSetLayout;
        static constexpr const char* name = "vkDestroyDescriptorSetLayout";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::FreeDescriptorSets> {
        using Type = PFN_vkFreeDescriptorSets;
        static constexpr const char* name = "vkFreeDescriptorSets";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::AllocateDescriptorSets> {
        using Type = PFN_vkAllocateDescriptorSets;
        static constexpr const char* name = "vkAllocateDescriptorSets";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyPipelineLayout> {
        using Type = PFN_vkDestroyPipelineLayout;
        static constexpr const char* name = "vkDestroyPipelineLayout";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreatePipelineLayout> {
        using Type = PFN_vkCreatePipelineLayout;
        static constexpr const char* name = "vkCreatePipelineLayout";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyPipeline> {
        using Type = PFN_vkDestroyPipeline;
        static constexpr const char* name = "vkDestroyPipeline";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateGraphicsPipelines> {
        using Type = PFN_vkCreateGraphicsPipelines;
        static constexpr const char* name = "vkCreateGraphicsPipelines";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyDescriptorPool> {
        using Type = PFN_vkDestroyDescriptorPool;
        static constexpr const char* name = "vkDestroyDescriptorPool";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateDescriptorPool> {
        using Type = PFN_vkCreateDescriptorPool;
        static constexpr const char* name = "vkCreateDescriptorPool";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::ResetDescriptorPool> {
        using Type = PFN_vkResetDescriptorPool;
        static constexpr const char* name = "vkResetDescriptorPool";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::CreateBufferView> {
        using Type = PFN_vkCreateBufferView;
        static constexpr const char* name = "vkCreateBufferView";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::DestroyBufferView> {
        using Type = PFN_vkDestroyBufferView;
        static constexpr const char* name = "vkDestroyBufferView";
    };

    template <>
    struct DeviceFunctionTraits<DeviceFunction::UpdateDescriptorSets> {
        using Type = PFN_vkUpdateDescriptorSets;
        static constexpr const char* name = "vkUpdateDescriptorSets";
    };

    using DeviceFunctionTable = FunctionTable<DeviceFunction, DeviceFunctionTraits, VkDevice, PFN_vkGetDeviceProcAddr>;
}