#pragma once
#include "Graphics/Common.h"

namespace Graphics {

    enum class DeviceFeature : size_t {
        // Vulkan 1.0 Features

        // Shader Pipeline Features
        GeometryShader = 0,
        TessellationShader,
        ShaderFloat64,

        // Sampling and Texture Features
        SamplerAnisotropy,
        FillModeNonSolid,
        TextureCompressionBc,
        TextureCompressionEtc2,

        // Drawing Features
        MultiDrawIndirect,
        WideLines,
        LargePoints,
        MultiViewport,

        // Depth and Blend Features
        DepthClamp,
        DepthBiasClamp,
        DualSrcBlend,

        // Storage and Atomic Features
        VertexPipelineStoresAndAtomics,
        FragmentStoresAndAtomics,

        // Vulkan 1.2 Features
        ShaderBufferInt64Atomics,    // Requires VkPhysicalDeviceShaderAtomicInt64Features
        ShaderSharedInt64Atomics,
        ShaderInt8,    // Requires VkPhysicalDeviceShaderFloat16Int8Features

        // Bindless Texture Features (Vulkan 1.2 - Descriptor Indexing)
        DescriptorBindingPartiallyBound,         // Allow partially bound descriptor arrays
        DescriptorBindingUpdateUnusedWhilePending,  // Allow updating unused descriptors while others are in use
        DescriptorBindingUniformBufferUpdateAfterBind,  // Allow updating uniform buffers after binding
        DescriptorBindingSampledImageUpdateAfterBind,   // Allow updating sampled images after binding
        DescriptorBindingStorageImageUpdateAfterBind,   // Allow updating storage images after binding
        DescriptorBindingVariableDescriptorCount,
        ShaderSampledImageArrayNonUniformIndexing,
        RuntimeDescriptorArray,                   // Allow variable-sized descriptor arrays
        DescriptorBindingStorageBufferUpdateAfterBind,
        DescriptorIndexing,

        //Robustness
        NullDescriptor,
        RobustBufferAccess,
        RobustImageAccess,

        // Mesh shading
        MeshShader,
        TaskShader,

        ShaderDrawParameters,

        Num
    };

    template<DeviceFeature F>
    struct DeviceFeatureTypeTrait;

    // Shader Pipeline Features
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::GeometryShader> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::TessellationShader> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::ShaderFloat64> { using Type = bool; };

    // Sampling and Texture Features
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::SamplerAnisotropy> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::FillModeNonSolid> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::TextureCompressionBc> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::TextureCompressionEtc2> { using Type = bool; };

    // Drawing Features
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::MultiDrawIndirect> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::WideLines> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::LargePoints> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::MultiViewport> { using Type = bool; };

    // Depth and Blend Features
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::DepthClamp> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::DepthBiasClamp> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::DualSrcBlend> { using Type = bool; };

    // Storage and Atomic Features
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::VertexPipelineStoresAndAtomics> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::FragmentStoresAndAtomics> { using Type = bool; };

    // Vulkan 1.2 Features
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::ShaderBufferInt64Atomics> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::ShaderSharedInt64Atomics> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::ShaderInt8> { using Type = bool; };

    // Bindless Texture Features
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::DescriptorBindingPartiallyBound> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::DescriptorBindingUpdateUnusedWhilePending> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::DescriptorBindingUniformBufferUpdateAfterBind> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::DescriptorBindingSampledImageUpdateAfterBind> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::DescriptorBindingStorageImageUpdateAfterBind> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::DescriptorBindingVariableDescriptorCount> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::ShaderSampledImageArrayNonUniformIndexing> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::RuntimeDescriptorArray> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::DescriptorBindingStorageBufferUpdateAfterBind> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::DescriptorIndexing> { using Type = bool; };
    
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::NullDescriptor> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::RobustBufferAccess> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::RobustImageAccess> { using Type = bool; };

    // Mesh shading
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::MeshShader> { using Type = bool; };
    template<> struct DeviceFeatureTypeTrait<DeviceFeature::TaskShader> { using Type = bool; };

    template<> struct DeviceFeatureTypeTrait<DeviceFeature::ShaderDrawParameters> { using Type = bool; };
}