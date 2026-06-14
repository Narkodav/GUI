#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"
#include "RenderPass.h"
#include "ShaderModule.h"
#include "DescriptorSet.h"

namespace Graphics
{
	class VertexInputAttributeDescription : public StructBase<VkVertexInputAttributeDescription, VertexInputAttributeDescription>
	{
		using Base = StructBase<VkVertexInputAttributeDescription, VertexInputAttributeDescription>;
	public:
		using Base::Base;

		constexpr VertexInputAttributeDescription(uint32_t location, uint32_t binding,
			Format format, uint32_t offset) : Base() {
			this->location = location;
			this->binding = binding;
			this->format = convertCEnum(format);
			this->offset = offset;
		}
		constexpr VertexInputAttributeDescription& setLocation(uint32_t location) {
			this->location = location;
			return *this;
		}
		constexpr VertexInputAttributeDescription& setBinding(uint32_t binding) {
			this->binding = binding;
			return *this;
		}
		constexpr VertexInputAttributeDescription& setFormat(Format format) {
			this->format = convertCEnum(format);
			return *this;
		}
		constexpr VertexInputAttributeDescription& setOffset(uint32_t offset) {
			this->offset = offset;
			return *this;
		}
	};

	class VertexInputBindingDescription : public StructBase<VkVertexInputBindingDescription, VertexInputBindingDescription>
	{
		using Base = StructBase<VkVertexInputBindingDescription, VertexInputBindingDescription>;
	public:
		using Base::Base;

		constexpr VertexInputBindingDescription(uint32_t binding, uint32_t stride, VertexInputRate inputRate) : Base() {
			this->binding = binding;
			this->stride = stride;
			this->inputRate = convertCEnum(inputRate);
		}
		constexpr VertexInputBindingDescription& setBinding(uint32_t binding) {
			this->binding = binding;
			return *this;
		}
		constexpr VertexInputBindingDescription& setStride(uint32_t stride) {
			this->stride = stride;
			return *this;
		}
		constexpr VertexInputBindingDescription& setInputRate(VertexInputRate inputRate) {
			this->inputRate = convertCEnum(inputRate);
			return *this;
		}
	};

	class StencilOpState : public StructBase<VkStencilOpState, StencilOpState>
	{
		using Base = StructBase<VkStencilOpState, StencilOpState>;
	public:
		using Base::Base;

		constexpr StencilOpState(StencilOp failOp, StencilOp passOp,
			StencilOp depthFailOp, CompareOp compareOp,
			uint32_t compareMask, uint32_t writeMask,
			uint32_t reference) : Base() {
			this->failOp = convertCEnum(failOp);
			this->passOp = convertCEnum(passOp);
			this->depthFailOp = convertCEnum(depthFailOp);
			this->compareOp = convertCEnum(compareOp);
			this->compareMask = compareMask;
			this->writeMask = writeMask;
			this->reference = reference;
		}
		constexpr StencilOpState& setFailOp(StencilOp failOp) {
			this->failOp = convertCEnum(failOp);
			return *this;
		}
		constexpr StencilOpState& setPassOp(StencilOp passOp) {
			this->passOp = convertCEnum(passOp);
			return *this;
		}
		constexpr StencilOpState& setDepthFailOp(StencilOp depthFailOp) {
			this->depthFailOp = convertCEnum(depthFailOp);
			return *this;
		}
		constexpr StencilOpState& setCompareOp(CompareOp compareOp) {
			this->compareOp = convertCEnum(compareOp);
			return *this;
		}
		constexpr StencilOpState& setCompareMask(uint32_t compareMask) {
			this->compareMask = compareMask;
			return *this;
		}
		constexpr StencilOpState& setWriteMask(uint32_t writeMask) {
			this->writeMask = writeMask;
			return *this;
		}
		constexpr StencilOpState& setReference(uint32_t reference) {
			this->reference = reference;
			return *this;
		}
	};

	class SpecializationMapEntry : public StructBase<VkSpecializationMapEntry, SpecializationMapEntry>
	{
		using Base = StructBase<VkSpecializationMapEntry, SpecializationMapEntry>;
	public:
		using Base::Base;
		constexpr SpecializationMapEntry& setConstantID(uint32_t constantID) {
			this->constantID = constantID;
			return *this;
		}
		constexpr SpecializationMapEntry& setOffset(uint32_t offset) {
			this->offset = offset;
			return *this;
		}
		constexpr SpecializationMapEntry& setSize(size_t size) {
			this->size = size;
			return *this;
		}
	};

	class SpecializationInfo : public StructBase<VkSpecializationInfo, SpecializationInfo>
	{
		using Base = StructBase<VkSpecializationInfo, SpecializationInfo>;
	public:
	using Base::Base;
		constexpr SpecializationInfo& setMapEntries(std::span<const SpecializationMapEntry> mapEntries) {
			this->mapEntryCount = mapEntries.size();
			this->pMapEntries = SpecializationMapEntry::underlyingCast(mapEntries.data());
			return *this;
		}
		constexpr SpecializationInfo& setData(const void* data, size_t size) {
			this->dataSize = size;
			this->pData = data;
			return *this;
		}
	};

	class PipelineRef : public BaseComponent<VkPipeline, PipelineRef>
	{
		using Base = BaseComponent<VkPipeline, PipelineRef>;
	public:
		using Base::Base;
		static inline const std::string s_typeName = "Pipeline";
	};

	class PipelineLayoutRef : public BaseComponent<VkPipelineLayout, PipelineLayoutRef>
	{
		using Base = BaseComponent<VkPipelineLayout, PipelineLayoutRef>;
	public:
		using Base::Base;
		static inline const std::string s_typeName = "Pipeline::PipelineLayout";
	};

	class PipelineLayoutCreateInfo : public StructBase<VkPipelineLayoutCreateInfo, PipelineLayoutCreateInfo>
	{
		using Base = StructBase<VkPipelineLayoutCreateInfo, PipelineLayoutCreateInfo>;
	public:
		using Base::Base;

		constexpr PipelineLayoutCreateInfo(std::span<const DescriptorSetLayout> setLayouts,
			std::span<const PushConstantRange> pushConstantRanges = {},
			Flags::PipelineLayoutCreate flags = Flags::PipelineLayoutCreate::Bits::None) : Base() {
			this->setLayoutCount = static_cast<uint32_t>(setLayouts.size());
			this->pSetLayouts = DescriptorSetLayout::underlyingCast(setLayouts.data());
			this->pushConstantRangeCount = pushConstantRanges.size();
			this->pPushConstantRanges = PushConstantRange::underlyingCast(pushConstantRanges.data());
			this->flags = flags;
		}

		constexpr PipelineLayoutCreateInfo& setFlags(Flags::PipelineLayoutCreate flags) {
			this->flags = flags;
			return *this;
		}

		constexpr PipelineLayoutCreateInfo& setSetLayouts(std::span<const DescriptorSetLayout> setLayouts) {
			this->setLayoutCount = static_cast<uint32_t>(setLayouts.size());
			this->pSetLayouts = DescriptorSetLayout::underlyingCast(setLayouts.data());
			return *this;
		}

		constexpr PipelineLayoutCreateInfo& setSetLayouts(const DescriptorSetLayout& setLayout) {
			this->setLayoutCount = 1;
			this->pSetLayouts = DescriptorSetLayout::underlyingCast(&setLayout);
			return *this;
		}

		constexpr PipelineLayoutCreateInfo& setPushConstantRanges(std::span<const PushConstantRange> pushConstantRanges) {
			this->pushConstantRangeCount = pushConstantRanges.size();
			this->pPushConstantRanges = PushConstantRange::underlyingCast(pushConstantRanges.data());
			return *this;
		}

		constexpr PipelineLayoutCreateInfo& setPushConstantRanges(const PushConstantRange& pushConstantRanges) {
			this->pushConstantRangeCount = 1;
			this->pPushConstantRanges = PushConstantRange::underlyingCast(&pushConstantRanges);
			return *this;
		}

		constexpr Flags::PipelineLayoutCreate getFlags() const { return this->flags; };
		constexpr std::span<const DescriptorSetLayoutRef> getSetLayouts() const {
			return std::span<const DescriptorSetLayoutRef>(DescriptorSetLayout::underlyingCast(this->pSetLayouts), this->setLayoutCount);
		};
		constexpr std::span<const PushConstantRange> getPushConstantRanges() const {
			return std::span<const PushConstantRange>(PushConstantRange::underlyingCast(this->pPushConstantRanges), this->pushConstantRangeCount);
		};
	};

	class PipelineLayout : public VerificatorComponent<VkPipelineLayout, PipelineLayoutRef>
	{
		using Base = VerificatorComponent<VkPipelineLayout, PipelineLayoutRef>;
	public:
		using Base::Base;		

		void create(const DeviceFunctionTable& functions,
			DeviceRef device, const PipelineLayoutCreateInfo& createInfo);
		void destroy(const DeviceFunctionTable& functions, DeviceRef device);
	};

	class PipelineShaderStageCreateInfo : public StructBase<VkPipelineShaderStageCreateInfo, PipelineShaderStageCreateInfo>
	{
		using Base = StructBase<VkPipelineShaderStageCreateInfo, PipelineShaderStageCreateInfo>;
	public:
		using Base::Base;

		constexpr PipelineShaderStageCreateInfo(Flags::ShaderStage::Bits stage,
			const ShaderModuleRef& module, const char* name,
			const SpecializationInfo& specializationInfo = SpecializationInfo(),
			Flags::PipelineShaderStageCreate flags = Flags::PipelineShaderStageCreate::Bits::None) : Base() {
			this->stage = static_cast<VkShaderStageFlagBits>(stage);
			this->module = module.getHandle();
			this->pName = name;
			this->pSpecializationInfo = specializationInfo.getUnderlyingPointer();
			this->flags = flags;
		}
		constexpr PipelineShaderStageCreateInfo& setStage(Flags::ShaderStage::Bits stage) {
			this->stage = static_cast<VkShaderStageFlagBits>(stage);
			return *this;
		}
		constexpr PipelineShaderStageCreateInfo& setModule(const ShaderModuleRef& module) {
			this->module = module.getHandle();
			return *this;
		}
		constexpr PipelineShaderStageCreateInfo& setName(const char* name) {
			this->pName = name;
			return *this;
		}
		constexpr PipelineShaderStageCreateInfo& setSpecializationInfo(const SpecializationInfo& specializationInfo) {
			this->pSpecializationInfo = specializationInfo.getUnderlyingPointer();
			return *this;
		}
		constexpr PipelineShaderStageCreateInfo& setFlags(Flags::PipelineShaderStageCreate flags) {
			this->flags = flags;
			return *this;
		}
	};

	class PipelineVertexInputStateCreateInfo : public StructBase<VkPipelineVertexInputStateCreateInfo, PipelineVertexInputStateCreateInfo>
	{
		using Base = StructBase<VkPipelineVertexInputStateCreateInfo, PipelineVertexInputStateCreateInfo>;
	public:
		using Base::Base;
		constexpr PipelineVertexInputStateCreateInfo(std::span<const VertexInputBindingDescription> bindingDescriptions,
			std::span<const VertexInputAttributeDescription> attributeDescriptions,
			Flags::PipelineVertexInputStateCreate flags = Flags::PipelineVertexInputStateCreate::Bits::None) : Base() {
			this->flags = flags;
			this->vertexBindingDescriptionCount = bindingDescriptions.size();
			this->pVertexBindingDescriptions = VertexInputBindingDescription::underlyingCast(bindingDescriptions.data());
			this->vertexAttributeDescriptionCount = attributeDescriptions.size();
			this->pVertexAttributeDescriptions = VertexInputAttributeDescription::underlyingCast(attributeDescriptions.data());
		}

		constexpr PipelineVertexInputStateCreateInfo& setFlags(Flags::PipelineVertexInputStateCreate flags) {
			this->flags = flags;
			return *this;
		}
		constexpr PipelineVertexInputStateCreateInfo& setVertexBindingDescriptions(std::span<const VertexInputBindingDescription> bindingDescriptions) {
			this->vertexBindingDescriptionCount = bindingDescriptions.size();
			this->pVertexBindingDescriptions = VertexInputBindingDescription::underlyingCast(bindingDescriptions.data());
			return *this;
		}
		constexpr PipelineVertexInputStateCreateInfo& setVertexAttributeDescriptions(std::span<const VertexInputAttributeDescription> attributeDescriptions) {
			this->vertexAttributeDescriptionCount = attributeDescriptions.size();
			this->pVertexAttributeDescriptions = VertexInputAttributeDescription::underlyingCast(attributeDescriptions.data());
			return *this;
		}
	};

	class PipelineInputAssemblyStateCreateInfo : public StructBase<VkPipelineInputAssemblyStateCreateInfo, PipelineInputAssemblyStateCreateInfo>
	{
		using Base = StructBase<VkPipelineInputAssemblyStateCreateInfo, PipelineInputAssemblyStateCreateInfo>;
	public:
		using Base::Base;
		constexpr PipelineInputAssemblyStateCreateInfo(PrimitiveTopology topology,
			bool primitiveRestartEnable,
			Flags::PipelineInputAssemblyStateCreate flags = Flags::PipelineInputAssemblyStateCreate::Bits::None) : Base() {
			this->primitiveRestartEnable = primitiveRestartEnable;
			this->topology = convertCEnum(topology);
			this->flags = flags;
		};

		constexpr PipelineInputAssemblyStateCreateInfo& setFlags(Flags::PipelineInputAssemblyStateCreate flags) {
			this->flags = flags;
			return *this;
		}
		constexpr PipelineInputAssemblyStateCreateInfo& setTopology(PrimitiveTopology topology) {
			this->topology = convertCEnum(topology);
			return *this;
		}
		constexpr PipelineInputAssemblyStateCreateInfo& setPrimitiveRestartEnable(bool primitiveRestartEnable) {
			this->primitiveRestartEnable = primitiveRestartEnable;
			return *this;
		}
	};

	class PipelineTessellationStateCreateInfo : public StructBase<VkPipelineTessellationStateCreateInfo, PipelineTessellationStateCreateInfo>
	{
		using Base = StructBase<VkPipelineTessellationStateCreateInfo, PipelineTessellationStateCreateInfo>;
	public:
		using Base::Base;
		constexpr PipelineTessellationStateCreateInfo& setFlags(Flags::PipelineTessellationStateCreate flags) {
			this->flags = flags;
			return *this;
		}
		constexpr PipelineTessellationStateCreateInfo& setPatchControlPoints(uint32_t patchControlPoints) {
			this->patchControlPoints = patchControlPoints;
			return *this;
		}
	};

	class PipelineViewportStateCreateInfo : public StructBase<VkPipelineViewportStateCreateInfo, PipelineViewportStateCreateInfo>
	{
		using Base = StructBase<VkPipelineViewportStateCreateInfo, PipelineViewportStateCreateInfo>;
	public:
		using Base::Base;

		constexpr PipelineViewportStateCreateInfo(std::span<const Viewport> viewports,
			std::span<const Rect2D> scissors,
			Flags::PipelineViewportStateCreate flags = Flags::PipelineViewportStateCreate::Bits::None) : Base() {
			this->flags = flags;
			this->viewportCount = viewports.size();
			this->pViewports = Viewport::underlyingCast(viewports.data());
			this->scissorCount = scissors.size();
			this->pScissors = Rect2D::underlyingCast(scissors.data());
		}

		constexpr PipelineViewportStateCreateInfo& setFlags(Flags::PipelineViewportStateCreate flags) {
			this->flags = flags;
			return *this;
		}
		constexpr PipelineViewportStateCreateInfo& setViewports(std::span<const Viewport> viewports) {
			this->viewportCount = viewports.size();
			this->pViewports = Viewport::underlyingCast(viewports.data());
			return *this;
		}
		constexpr PipelineViewportStateCreateInfo& setScissors(std::span<const Rect2D> scissors) {
			this->scissorCount = scissors.size();
			this->pScissors = Rect2D::underlyingCast(scissors.data());
			return *this;
		}

		// For dynamic state
		constexpr PipelineViewportStateCreateInfo& setViewportCount(uint32_t count) {
			this->viewportCount = count;
			this->pViewports = nullptr;
			return *this;
		}
		constexpr PipelineViewportStateCreateInfo& setScissorCount(uint32_t count) {
			this->scissorCount = count;
			this->pScissors = nullptr;
			return *this;
		}
	};

	class PipelineRasterizationStateCreateInfo : public StructBase<VkPipelineRasterizationStateCreateInfo, PipelineRasterizationStateCreateInfo>
	{
		using Base = StructBase<VkPipelineRasterizationStateCreateInfo, PipelineRasterizationStateCreateInfo>;
	public:
		using Base::Base;

		constexpr PipelineRasterizationStateCreateInfo(bool depthClampEnable,
			bool rasterizerDiscardEnable, PolygonMode polygonMode,
			Flags::CullMode cullMode, FrontFace frontFace,
			bool depthBiasEnable, float depthBiasConstantFactor,
			float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
			Flags::PipelineRasterizationStateCreate flags = Flags::PipelineRasterizationStateCreate::Bits::None) : Base() {
			this->flags = flags;
			this->depthClampEnable = depthClampEnable;
			this->rasterizerDiscardEnable = rasterizerDiscardEnable;
			this->polygonMode = convertCEnum(polygonMode);
			this->cullMode = cullMode;
			this->frontFace = convertCEnum(frontFace);
			this->depthBiasEnable = depthBiasEnable;
			this->depthBiasConstantFactor = depthBiasConstantFactor;
			this->depthBiasClamp = depthBiasClamp;
			this->depthBiasSlopeFactor = depthBiasSlopeFactor;
			this->lineWidth = lineWidth;
		};

		constexpr PipelineRasterizationStateCreateInfo& setFlags(Flags::PipelineRasterizationStateCreate flags) {
			this->flags = flags;
			return *this;
		}
		constexpr PipelineRasterizationStateCreateInfo& setDepthClampEnable(bool depthClampEnable) {
			this->depthClampEnable = depthClampEnable;
			return *this;
		}
		constexpr PipelineRasterizationStateCreateInfo& setRasterizerDiscardEnable(bool rasterizerDiscardEnable) {
			this->rasterizerDiscardEnable = rasterizerDiscardEnable;
			return *this;
		}
		constexpr PipelineRasterizationStateCreateInfo& setPolygonMode(PolygonMode polygonMode) {
			this->polygonMode = convertCEnum(polygonMode);
			return *this;
		}
		constexpr PipelineRasterizationStateCreateInfo& setCullMode(Flags::CullMode cullMode) {
			this->cullMode = cullMode;
			return *this;
		}
		constexpr PipelineRasterizationStateCreateInfo& setFrontFace(FrontFace frontFace) {
			this->frontFace = convertCEnum(frontFace);
			return *this;
		}
		constexpr PipelineRasterizationStateCreateInfo& setDepthBiasEnable(bool depthBiasEnable) {
			this->depthBiasEnable = depthBiasEnable;
			return *this;
		}
		constexpr PipelineRasterizationStateCreateInfo& setDepthBiasConstantFactor(float depthBiasConstantFactor) {
			this->depthBiasConstantFactor = depthBiasConstantFactor;
			return *this;
		}
		constexpr PipelineRasterizationStateCreateInfo& setDepthBiasClamp(float depthBiasClamp) {
			this->depthBiasClamp = depthBiasClamp;
			return *this;
		}
		constexpr PipelineRasterizationStateCreateInfo& setDepthBiasSlopeFactor(float depthBiasSlopeFactor) {
			this->depthBiasSlopeFactor = depthBiasSlopeFactor;
			return *this;
		}
		constexpr PipelineRasterizationStateCreateInfo& setLineWidth(float lineWidth) {
			this->lineWidth = lineWidth;
			return *this;
		}
	};

	class PipelineMultisampleStateCreateInfo : public StructBase<VkPipelineMultisampleStateCreateInfo, PipelineMultisampleStateCreateInfo>
	{
		using Base = StructBase<VkPipelineMultisampleStateCreateInfo, PipelineMultisampleStateCreateInfo>;
	public:
		using Base::Base;

		constexpr PipelineMultisampleStateCreateInfo(bool sampleShadingEnable,
			Flags::SampleCount::Bits rasterizationSamples,
			float minSampleShading, std::span<const SampleMask> sampleMask,
			bool alphaToCoverageEnable, bool alphaToOneEnable,
			Flags::PipelineMultisampleStateCreate flags = Flags::PipelineMultisampleStateCreate::Bits::None) : Base() {
			this->flags = flags;
			this->sampleShadingEnable = sampleShadingEnable;
			this->rasterizationSamples = static_cast<VkSampleCountFlagBits>(rasterizationSamples);
			this->minSampleShading = minSampleShading;
			this->pSampleMask = sampleMask.data();
			this->alphaToCoverageEnable = alphaToCoverageEnable;
			this->alphaToOneEnable = alphaToOneEnable;
		};
		constexpr PipelineMultisampleStateCreateInfo& setFlags(Flags::PipelineMultisampleStateCreate flags) {
			this->flags = flags;
			return *this;
		}
		constexpr PipelineMultisampleStateCreateInfo& setRasterizationSamples(Flags::SampleCount::Bits rasterizationSamples) {
			this->rasterizationSamples = static_cast<VkSampleCountFlagBits>(rasterizationSamples);
			return *this;
		}
		constexpr PipelineMultisampleStateCreateInfo& setSampleShadingEnable(bool sampleShadingEnable) {
			this->sampleShadingEnable = sampleShadingEnable;
			return *this;
		}
		constexpr PipelineMultisampleStateCreateInfo& setMinSampleShading(float minSampleShading) {
			this->minSampleShading = minSampleShading;
			return *this;
		}
		constexpr PipelineMultisampleStateCreateInfo& setSampleMask(std::span<const SampleMask> sampleMask) {
			this->pSampleMask = sampleMask.data();
			return *this;
		}
		constexpr PipelineMultisampleStateCreateInfo& setAlphaToCoverageEnable(bool alphaToCoverageEnable) {
			this->alphaToCoverageEnable = alphaToCoverageEnable;
			return *this;
		}
		constexpr PipelineMultisampleStateCreateInfo& setAlphaToOneEnable(bool alphaToOneEnable) {
			this->alphaToOneEnable = alphaToOneEnable;
			return *this;
		}
	};

	class PipelineDepthStencilStateCreateInfo : public StructBase<VkPipelineDepthStencilStateCreateInfo, PipelineDepthStencilStateCreateInfo>
	{
		using Base = StructBase<VkPipelineDepthStencilStateCreateInfo, PipelineDepthStencilStateCreateInfo>;
	public:
		using Base::Base;
		constexpr PipelineDepthStencilStateCreateInfo(const StencilOpState& front, const StencilOpState& back,
			CompareOp depthCompareOp, bool depthBoundsTestEnable,
			bool stencilTestEnable, bool depthTestEnable, bool depthWriteEnable,
			float minDepthBounds, float maxDepthBounds,
			Flags::PipelineDepthStencilStateCreate flags = Flags::PipelineDepthStencilStateCreate::Bits::None) : Base() {
			this->flags = flags;
			this->depthTestEnable = depthTestEnable;
			this->depthWriteEnable = depthWriteEnable;
			this->depthCompareOp = convertCEnum(depthCompareOp);
			this->depthBoundsTestEnable = depthBoundsTestEnable;
			this->stencilTestEnable = stencilTestEnable;
			this->front = front.getStruct();
			this->back = back.getStruct();
			this->minDepthBounds = minDepthBounds;
			this->maxDepthBounds = maxDepthBounds;
		};

		constexpr PipelineDepthStencilStateCreateInfo& setFlags(Flags::PipelineDepthStencilStateCreate flags) {
			this->flags = flags;
			return *this;
		}
		constexpr PipelineDepthStencilStateCreateInfo& setDepthTestEnable(bool depthTestEnable) {
			this->depthTestEnable = depthTestEnable;
			return *this;
		}
		constexpr PipelineDepthStencilStateCreateInfo& setDepthWriteEnable(bool depthWriteEnable) {
			this->depthWriteEnable = depthWriteEnable;
			return *this;
		}
		constexpr PipelineDepthStencilStateCreateInfo& setDepthCompareOp(CompareOp depthCompareOp) {
			this->depthCompareOp = convertCEnum(depthCompareOp);
			return *this;
		}
		constexpr PipelineDepthStencilStateCreateInfo& setDepthBoundsTestEnable(bool depthBoundsTestEnable) {
			this->depthBoundsTestEnable = depthBoundsTestEnable;
			return *this;
		}
		constexpr PipelineDepthStencilStateCreateInfo& setStencilTestEnable(bool stencilTestEnable) {
			this->stencilTestEnable = stencilTestEnable;
			return *this;
		}
		constexpr PipelineDepthStencilStateCreateInfo& setFront(const StencilOpState& front) {
			this->front = front.getStruct();
			return *this;
		}
		constexpr PipelineDepthStencilStateCreateInfo& setBack(const StencilOpState& back) {
			this->back = back.getStruct();
			return *this;
		}
		constexpr PipelineDepthStencilStateCreateInfo& setMinDepthBounds(float minDepthBounds) {
			this->minDepthBounds = minDepthBounds;
			return *this;
		}
		constexpr PipelineDepthStencilStateCreateInfo& setMaxDepthBounds(float maxDepthBounds) {
			this->maxDepthBounds = maxDepthBounds;
			return *this;
		}
	};

	class PipelineColorBlendAttachmentState : public StructBase<VkPipelineColorBlendAttachmentState, PipelineColorBlendAttachmentState>
	{
		using Base = StructBase<VkPipelineColorBlendAttachmentState, PipelineColorBlendAttachmentState>;
	public:
		using Base::Base;

		constexpr PipelineColorBlendAttachmentState(bool blendEnable,
			BlendFactor srcColorBlendFactor, BlendFactor dstColorBlendFactor,
			BlendOp colorBlendOp, BlendFactor srcAlphaBlendFactor,
			BlendFactor dstAlphaBlendFactor, BlendOp alphaBlendOp,
			Flags::ColorComponent colorWriteMask) : Base() {
			this->blendEnable = blendEnable;
			this->srcColorBlendFactor = convertCEnum(srcColorBlendFactor);
			this->dstColorBlendFactor = convertCEnum(dstColorBlendFactor);
			this->colorBlendOp = convertCEnum(colorBlendOp);
			this->srcAlphaBlendFactor = convertCEnum(srcAlphaBlendFactor);
			this->dstAlphaBlendFactor = convertCEnum(dstAlphaBlendFactor);
			this->alphaBlendOp = convertCEnum(alphaBlendOp);
			this->colorWriteMask = colorWriteMask;
		};

		constexpr PipelineColorBlendAttachmentState& setBlendEnable(bool blendEnable) {
			this->blendEnable = blendEnable;
			return *this;
		}
		constexpr PipelineColorBlendAttachmentState& setSrcColorBlendFactor(BlendFactor srcColorBlendFactor) {
			this->srcColorBlendFactor = convertCEnum(srcColorBlendFactor);
			return *this;
		}
		constexpr PipelineColorBlendAttachmentState& setDstColorBlendFactor(BlendFactor dstColorBlendFactor) {
			this->dstColorBlendFactor = convertCEnum(dstColorBlendFactor);
			return *this;
		}
		constexpr PipelineColorBlendAttachmentState& setColorBlendOp(BlendOp colorBlendOp) {
			this->colorBlendOp = convertCEnum(colorBlendOp);
			return *this;
		}
		constexpr PipelineColorBlendAttachmentState& setSrcAlphaBlendFactor(BlendFactor srcAlphaBlendFactor) {
			this->srcAlphaBlendFactor = convertCEnum(srcAlphaBlendFactor);
			return *this;
		}
		constexpr PipelineColorBlendAttachmentState& setDstAlphaBlendFactor(BlendFactor dstAlphaBlendFactor) {
			this->dstAlphaBlendFactor = convertCEnum(dstAlphaBlendFactor);
			return *this;
		}
		constexpr PipelineColorBlendAttachmentState& setAlphaBlendOp(BlendOp alphaBlendOp) {
			this->alphaBlendOp = convertCEnum(alphaBlendOp);
			return *this;
		}
		constexpr PipelineColorBlendAttachmentState& setColorWriteMask(Flags::ColorComponent colorWriteMask) {
			this->colorWriteMask = colorWriteMask;
			return *this;
		}
	};

	class PipelineColorBlendStateCreateInfo : public StructBase<VkPipelineColorBlendStateCreateInfo, PipelineColorBlendStateCreateInfo>
	{
		using Base = StructBase<VkPipelineColorBlendStateCreateInfo, PipelineColorBlendStateCreateInfo>;
	public:
		using Base::Base;

		constexpr PipelineColorBlendStateCreateInfo(std::span<const PipelineColorBlendAttachmentState> attachments,
			bool logicOpEnable, LogicOp logicOp, const Color& blendConstants,
			Flags::PipelineColorBlendStateCreate flags = Flags::PipelineColorBlendStateCreate::Bits::None) : Base() {
			this->flags = flags;
			this->logicOpEnable = logicOpEnable;
			this->logicOp = convertCEnum(logicOp);
			this->attachmentCount = attachments.size();
			this->pAttachments = PipelineColorBlendAttachmentState::underlyingCast(attachments.data());
			std::memcpy(this->blendConstants, blendConstants.getStruct().data(), sizeof(float) * 4);
		}
		constexpr PipelineColorBlendStateCreateInfo& setFlags(Flags::PipelineColorBlendStateCreate flags) {
			this->flags = flags;
			return *this;
		}
		constexpr PipelineColorBlendStateCreateInfo& setLogicOpEnable(bool logicOpEnable) {
			this->logicOpEnable = logicOpEnable;
			return *this;
		}
		constexpr PipelineColorBlendStateCreateInfo& setLogicOp(LogicOp logicOp) {
			this->logicOp = convertCEnum(logicOp);
			return *this;
		}
		constexpr PipelineColorBlendStateCreateInfo& setAttachments(std::span<const PipelineColorBlendAttachmentState> attachments) {
			this->attachmentCount = attachments.size();
			this->pAttachments = PipelineColorBlendAttachmentState::underlyingCast(attachments.data());
			return *this;
		}
		constexpr PipelineColorBlendStateCreateInfo& setAttachments(const PipelineColorBlendAttachmentState& attachment) {
			this->attachmentCount = 1;
			this->pAttachments = PipelineColorBlendAttachmentState::underlyingCast(&attachment);
			return *this;
		}
		constexpr PipelineColorBlendStateCreateInfo& setBlendConstants(const Color& blendConstants) {
			const auto& constants = blendConstants.getStruct();
			this->blendConstants[0] = constants[0];
			this->blendConstants[1] = constants[1];
			this->blendConstants[2] = constants[2];
			this->blendConstants[3] = constants[3];
			return *this;
		}
	};

	class PipelineDynamicStateCreateInfo : public StructBase<VkPipelineDynamicStateCreateInfo, PipelineDynamicStateCreateInfo>
	{
		using Base = StructBase<VkPipelineDynamicStateCreateInfo, PipelineDynamicStateCreateInfo>;
	public:
		using Base::Base;

		constexpr PipelineDynamicStateCreateInfo(std::span<const DynamicState> dynamicStates,
			Flags::PipelineDynamicStateCreate flags = Flags::PipelineDynamicStateCreate::Bits::None) : Base() {
			this->flags = flags;
			this->dynamicStateCount = dynamicStates.size();
			this->pDynamicStates = convertCEnum(dynamicStates.data());
		}

		constexpr PipelineDynamicStateCreateInfo& setFlags(Flags::PipelineDynamicStateCreate flags) {
			this->flags = flags;
			return *this;
		}
		constexpr PipelineDynamicStateCreateInfo& setDynamicStates(std::span<const DynamicState> dynamicStates) {
			this->dynamicStateCount = dynamicStates.size();
			this->pDynamicStates = convertCEnum(dynamicStates.data());
			return *this;
		}
	};

	class PipelineRenderingCreateInfo : public StructBase<VkPipelineRenderingCreateInfo, PipelineRenderingCreateInfo>
	{
		using Base = StructBase<VkPipelineRenderingCreateInfo, PipelineRenderingCreateInfo>;
	public:
		using Base::Base;

		constexpr PipelineRenderingCreateInfo& setViewMask(uint32_t mask) {
			this->viewMask = mask;
			return *this;
		}
		constexpr PipelineRenderingCreateInfo& setColorAttachmentFormats(std::span<const Format> formats) {
			this->colorAttachmentCount = formats.size();
			this->pColorAttachmentFormats = convertCEnum(formats.data());
			return *this;
		}
		constexpr PipelineRenderingCreateInfo& setColorAttachmentFormats(const Format& format) {
			this->colorAttachmentCount = 1;
			this->pColorAttachmentFormats = convertCEnum(&format);
			return *this;
		}
		constexpr PipelineRenderingCreateInfo& setDepthAttachmentFormat(const Format& format) {
			this->depthAttachmentFormat = convertCEnum(format);
			return *this;
		}
		constexpr PipelineRenderingCreateInfo& setStencilAttachmentFormat(const Format& format) {
			this->stencilAttachmentFormat = convertCEnum(format);
			return *this;
		}
	};

	class PipelineCreateInfo : public StructBase<VkGraphicsPipelineCreateInfo, PipelineCreateInfo>
	{
		using Base = StructBase<VkGraphicsPipelineCreateInfo, PipelineCreateInfo>;
	public:
		using Base::Base;
		constexpr PipelineCreateInfo& setFlags(Flags::PipelineCreate flags) {
			this->flags = flags;
			return *this;
		}
		constexpr PipelineCreateInfo& setStages(std::span<const PipelineShaderStageCreateInfo> stages) {
			this->stageCount = stages.size();
			this->pStages = PipelineShaderStageCreateInfo::underlyingCast(stages.data());
			return *this;
		}
		constexpr PipelineCreateInfo& setVertexInputState(const PipelineVertexInputStateCreateInfo& vertexInputState) {
			this->pVertexInputState = vertexInputState.getUnderlyingPointer();
			return *this;
		}
		constexpr PipelineCreateInfo& setInputAssemblyState(const PipelineInputAssemblyStateCreateInfo& inputAssemblyState) {
			this->pInputAssemblyState = inputAssemblyState.getUnderlyingPointer();
			return *this;
		}
		constexpr PipelineCreateInfo& setTessellationState(const PipelineTessellationStateCreateInfo& tessellationState) {
			this->pTessellationState = tessellationState.getUnderlyingPointer();
			return *this;
		}
		constexpr PipelineCreateInfo& setViewportState(const PipelineViewportStateCreateInfo& viewportState) {
			this->pViewportState = viewportState.getUnderlyingPointer();
			return *this;
		}
		constexpr PipelineCreateInfo& setRasterizationState(const PipelineRasterizationStateCreateInfo& rasterizationState) {
			this->pRasterizationState = rasterizationState.getUnderlyingPointer();
			return *this;
		}
		constexpr PipelineCreateInfo& setMultisampleState(const PipelineMultisampleStateCreateInfo& multisampleState) {
			this->pMultisampleState = multisampleState.getUnderlyingPointer();
			return *this;
		}
		constexpr PipelineCreateInfo& setDepthStencilState(const PipelineDepthStencilStateCreateInfo& depthStencilState) {
			this->pDepthStencilState = depthStencilState.getUnderlyingPointer();
			return *this;
		}
		constexpr PipelineCreateInfo& setColorBlendState(const PipelineColorBlendStateCreateInfo& colorBlendState) {
			this->pColorBlendState = colorBlendState.getUnderlyingPointer();
			return *this;
		}
		constexpr PipelineCreateInfo& setDynamicState(const PipelineDynamicStateCreateInfo& dynamicState) {
			this->pDynamicState = dynamicState.getUnderlyingPointer();
			return *this;
		}
		constexpr PipelineCreateInfo& setLayout(PipelineLayoutRef layout) {
			this->layout = layout.getHandle();
			return *this;
		}
		constexpr PipelineCreateInfo& setRenderPass(RenderPassRef renderPass) {
			this->renderPass = renderPass.getHandle();
			return *this;
		}
		constexpr PipelineCreateInfo& setRenderPass(std::nullptr_t) {
			this->renderPass = nullptr;
			return *this;
		}
		constexpr PipelineCreateInfo& setSubpass(uint32_t subpass) {
			this->subpass = subpass;
			return *this;
		}
		constexpr PipelineCreateInfo& setBasePipelineHandle(PipelineRef basePipeline) {
			this->basePipelineHandle = basePipeline.getHandle();
			return *this;
		}
		constexpr PipelineCreateInfo& setBasePipelineIndex(int32_t basePipelineIndex) {
			this->basePipelineIndex = basePipelineIndex;
			return *this;
		}
	};

	class Pipeline : public VerificatorComponent<VkPipeline, PipelineRef>
	{
		using Base = VerificatorComponent<VkPipeline, PipelineRef>;
	public:
		using Base::Base;

		void create(const DeviceFunctionTable& functions,
			DeviceRef device, const PipelineCreateInfo& createInfo);
		void destroy(const DeviceFunctionTable& functions, DeviceRef device);

		static std::vector<Pipeline> create(const DeviceFunctionTable& functions,
			DeviceRef device, std::span<const PipelineCreateInfo> createInfos);
	};
}
