#pragma once
#include "Graphics/Common.h"

namespace Graphics {

    enum class DeviceProperty : size_t {
        //basic
        ApiVersion,                                // expects std::any containing uint32_t
        DriverVersion,                             // expects std::any containing uint32_t
        VendorId,                                  // expects std::any containing uint32_t
        PhysicalDeviceId,                         // expects std::any containing uint32_t
        PhysicalDeviceType,                       // expects std::any containing PhysicalDeviceType
        PhysicalDeviceName,                       // expects std::any containing std::string

        // Core Limits
        MaxImageDimension2d,                     // expects std::any containing uint32_t
        MaxImageDimension3d,                     // expects std::any containing uint32_t
        MaxImageDimensionCube,                   // expects std::any containing uint32_t
        MaxImageArrayLayers,                     // expects std::any containing uint32_t

        // Descriptor Limits
        MaxBoundDescriptorSets,                  // expects std::any containing uint32_t
        MaxPerStageDescriptorSamplers,          // expects std::any containing uint32_t
        MaxPerStageDescriptorUniformBuffers,   // expects std::any containing uint32_t
        MaxPerStageDescriptorStorageBuffers,   // expects std::any containing uint32_t

        // Buffer Limits
        MaxStorageBufferRange,                   // expects std::any containing uint64_t (VkDeviceSize)
        MaxUniformBufferRange,                   // expects std::any containing uint64_t (VkDeviceSize)
        MinUniformBufferOffsetAlignment,        // expects std::any containing uint64_t (VkDeviceSize)
        MinStorageBufferOffsetAlignment,        // expects std::any containing uint64_t (VkDeviceSize)

        // Viewport/Scissor
        MaxViewportDimensions,                    // expects std::any containing std::array<uint32_t, 2>
        MaxViewports,                              // expects std::any containing uint32_t
        ViewportBoundsRange,                      // expects std::any containing std::array<float, 2>
        ViewportSubPixelBits,                    // expects std::any containing uint32_t

        // Sample Limits
        MaxFramebufferLayers,                     // expects std::any containing uint32_t
        MaxSampleMaskWords,                      // expects std::any containing uint32_t
        MaxColorAttachments,                      // expects std::any containing uint32_t

        // Memory Limits
        MaxMemoryAllocationCount,                // expects std::any containing uint32_t
        MaxSamplerAllocationCount,               // expects std::any containing uint32_t
        BufferImageGranularity,                   // expects std::any containing uint64_t (VkDeviceSize)
        SparseAddressSpaceSize,                  // expects std::any containing uint64_t (VkDeviceSize)

        // Quality Settings
        MaxSamplerAnisotropy,                     // expects std::any containing float
        MaxSamplerLodBias,                       // expects std::any containing float
        TimestampPeriod,                           // expects std::any containing float

        Num,
    };

    enum class PhysicalDeviceType
    {
        Other         = VK_PHYSICAL_DEVICE_TYPE_OTHER,
        IntegratedGpu = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
        DiscreteGpu   = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
        VirtualGpu    = VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
        Cpu           = VK_PHYSICAL_DEVICE_TYPE_CPU
    };

    template<DeviceProperty P>
    struct DevicePropertyTypeTrait;

    // Basic properties
    template<> struct DevicePropertyTypeTrait<DeviceProperty::ApiVersion> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::DriverVersion> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::VendorId> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::PhysicalDeviceId> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::PhysicalDeviceType> { using Type = PhysicalDeviceType; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::PhysicalDeviceName> { using Type = std::string_view; };

    // Core Limits
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxImageDimension2d> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxImageDimension3d> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxImageDimensionCube> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxImageArrayLayers> { using Type = uint32_t; };

    // Descriptor Limits
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxBoundDescriptorSets> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxPerStageDescriptorSamplers> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxPerStageDescriptorUniformBuffers> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxPerStageDescriptorStorageBuffers> { using Type = uint32_t; };

    // Buffer Limits
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxStorageBufferRange> { using Type = DeviceSize; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxUniformBufferRange> { using Type = DeviceSize; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MinUniformBufferOffsetAlignment> { using Type = DeviceSize; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MinStorageBufferOffsetAlignment> { using Type = DeviceSize; };

    // Viewport/Scissor
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxViewportDimensions> { using Type = std::span<uint32_t, 2>; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxViewports> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::ViewportBoundsRange> { using Type = std::span<float, 2>; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::ViewportSubPixelBits> { using Type = uint32_t; };

    // Sample Limits
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxFramebufferLayers> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxSampleMaskWords> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxColorAttachments> { using Type = uint32_t; };

    // Memory Limits
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxMemoryAllocationCount> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxSamplerAllocationCount> { using Type = uint32_t; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::BufferImageGranularity> { using Type = DeviceSize; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::SparseAddressSpaceSize> { using Type = DeviceSize; };

    // Quality Settings
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxSamplerAnisotropy> { using Type = float; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::MaxSamplerLodBias> { using Type = float; };
    template<> struct DevicePropertyTypeTrait<DeviceProperty::TimestampPeriod> { using Type = float; };

}