#pragma once
#include "Graphics/HandleTypes/PhysicalDevice.h"
#include "Graphics/InstanceFunctionTable.h"
#include "Graphics/Enums.h"
#include "Graphics./HandleTypes/SwapChain.h"
#include "Graphics/HandleTypes/FrameBuffer.h"
#include "Graphics/HandleTypes/ShaderModule.h"
#include "Graphics/HandleTypes/CommandBuffer.h"
#include "Structs.h"

// Utility functions that abstract some common operations
namespace Graphics::Utility
{    
    uint32_t findMemoryTypeFirstFit(
        const PhysicalDeviceMemoryProperties& memProperties,
        uint32_t typeFilter,
        Flags::MemoryProperty requiredProperties,
        Flags::MemoryProperty forbiddenProperties = Flags::MemoryProperty::Bits::None);

	//returns a memory type index based on type filter and properties requirements
    std::vector<uint32_t> findCompatibleMemoryTypes(
        const PhysicalDeviceMemoryProperties& memProperties,
        uint32_t typeFilter,
        Flags::MemoryProperty requiredProperties,
        Flags::MemoryProperty forbiddenProperties = Flags::MemoryProperty::Bits::None);

    void sortMemoryTypesByScore(
        const PhysicalDeviceMemoryProperties& memProperties,
        std::vector<uint32_t>& memoryTypeIndices,
        std::array<int64_t, Flags::MemoryProperty::bitCount()> weights);

    void sortMemoryTypesByScore(
        const PhysicalDeviceMemoryProperties& memProperties,
        std::vector<uint32_t>& memoryTypeIndices,
        std::function<int(Flags::MemoryProperty flags)> sorter);

    //returns a supported format based on tiling and features requirements
    Format findSupportedFormat(const InstanceFunctionTable& functions,
        const PhysicalDevice& device, const std::vector<Format>& candidates,
        ImageTiling tiling, Flags::FormatFeature features);

	//finds a commonly used depth format
    Format findDepthFormat(const InstanceFunctionTable& functions,
        const PhysicalDevice& device);

	//calculates the number of mip levels for a given extent
    uint32_t calculateMipLevels(const Extent2D& extent);

	//chooses a swap chain extent based on surface capabilities and desired frame buffer extent
	Extent2D chooseExtent(const SurfaceCapabilities& capabilities, const Extent2D& frameBufferExtent);

    RenderPassData createColorDepthRenderPass(const DeviceFunctionTable& deviceFunctions, const DeviceRef& device,
        std::span<SurfaceFormat> formats, std::span<PresentMode> presentModes, Format depthFormat);

	//creates a basic swap chain with image views and frame buffers, optionally with depth buffer
    SwapChainData createBasicSwapChain(
        const InstanceFunctionTable& functions,
        const DeviceFunctionTable& deviceFunctions,
        const PhysicalDevice& physicalDevice,
        const DeviceRef& device,
        const SurfaceRef& surface,
        const RenderPassRef& renderPass,
        const Extent2D& preferredExtent,
        Format preferredFormat = Format::B8G8R8A8Srgb,
        Format preferredDepthFormat = Format::D32Sfloat,
        ColorSpace preferredColorSpace = ColorSpace::SrgbNonlinear,
        Flags::ImageUsage preferredImageUsage = Flags::ImageUsage::Bits::ColorAttachment,
        PresentMode preferredPresentMode = PresentMode::Mailbox,
        uint32_t desiredImageCount = 2,
        uint32_t desiredImageArrayLayerCount = 1);

    void recreateBasicSwapChain(SwapChainData& data,
        const DeviceFunctionTable& deviceFunctions,
        DeviceRef device, RenderPassRef renderPass,
        const PhysicalDeviceMemoryProperties& physicalDeviceMemoryProperties,
        const Extent2D& preferredExtent);

    void destroySwapChainData(const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device, SwapChainData& data);

    void createBasicGraphicsPipeline(GraphicsPipelineData& data,
        const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device,
        const RenderPassRef& renderPass,
        uint32_t subpassIndex,
        PrimitiveTopology desiredTopology,
        PolygonMode desiredPolygonMode, Flags::CullMode desiredCullMode,
        FrontFace desiredFrontFace, CompareOp depthCompareOp,
        bool depthTestEnable, bool depthWriteEnable);

    ShaderModuleData createShaderModules(const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device, std::span<const std::string> shaderFilePaths);

    ShaderModuleData createShaderModules(const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device, std::initializer_list<const std::string> shaderFilePaths);

    std::vector<PipelineShaderStageCreateInfo> createShaderStageInfos(const ShaderModuleData& shaderData);

    void blitImage(const DeviceFunctionTable& deviceFunctions, const CommandBuffer& commandBuffer,
        const ImageRef& srcImage, const ImageRef& dstImage, const Extent3D& srcExtent, const Extent3D& dstExtent);

    void copyBufferToImage(const DeviceFunctionTable& deviceFunctions, CommandBuffer& commandBuffer,
        const BufferRef& srcBuffer, const ImageRef& dstImage, const Extent3D& dstExtent,
        ImageLayout srcLayout, ImageLayout dstLayout);

    void transferData(const DeviceFunctionTable& deviceFunctions, CommandBuffer& commandBuffer,
        const BufferRef& srcBuffer, const BufferRef& dstBuffer, size_t size, size_t srcOffset = 0, size_t dstOffset = 0);

    uint32_t calculateMipLevelCount(uint32_t width, uint32_t height);

    MemoryRequirements createBufferMemoryPairFirstFit(const DeviceFunctionTable& deviceFunctions, DeviceRef device,
        const PhysicalDeviceMemoryProperties& deviceMemoryProps, Buffer& buffer, Memory& memory,
        size_t size, Flags::BufferUsage bufferUsage, Flags::MemoryProperty requiredProperties,
        Flags::MemoryProperty forbiddenProperties = Flags::MemoryProperty::Bits::None, 
        SharingMode sharingMode = SharingMode::Exclusive);
}