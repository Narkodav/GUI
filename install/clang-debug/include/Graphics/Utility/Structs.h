#pragma once
#include "../HandleTypes/PhysicalDevice.h"
#include "../InstanceFunctionTable.h"
#include "../Enums.h"
#include "../HandleTypes/SwapChain.h"
#include "../HandleTypes/FrameBuffer.h"
#include "../HandleTypes/Pipeline.h"
#include "../HandleTypes/DescriptorSet.h"

namespace Graphics::Utility
{
    struct SwapChainData {
        SwapChain swapChain;
        SwapChainCreateInfo swapChainInfo;

        std::vector<ImageRef> swapChainImages;

        std::vector<ImageViewCreateInfo> swapChainImageViewCreateInfos;
        std::vector<FrameBufferCreateInfo> swapChainFrameBufferCreateInfos;
        std::vector<ImageView> swapChainImageViews;
        std::vector<FrameBuffer> swapChainFrameBuffers;

        std::vector<std::vector<ImageViewRef>> attachmentRefs;

        Image depthImage;
        Memory depthImageMemory;
        ImageView depthImageView;

        ImageCreateInfo depthImageCreateInfo;
        MemoryAllocateInfo depthImageMemoryCreateInfo;
        ImageViewCreateInfo depthImageViewCreateInfo;
    };

    struct GraphicsPipelineData {
        Pipeline graphicsPipeline;
        PipelineCreateInfo graphicsPipelineInfo;

        PipelineVertexInputStateCreateInfo vertexInputState;
		PipelineInputAssemblyStateCreateInfo inputAssemblyState;
		PipelineTessellationStateCreateInfo tessellationState;
		PipelineViewportStateCreateInfo viewportState;
		PipelineRasterizationStateCreateInfo rasterizationState;
		PipelineMultisampleStateCreateInfo multisampleState;
		PipelineDepthStencilStateCreateInfo depthStencilState;
        PipelineColorBlendStateCreateInfo colorBlendState;
		PipelineDynamicStateCreateInfo dynamicState;
        std::vector<PipelineColorBlendAttachmentState> colorBlendAttachmentStates;

        //these are populated beforehand
        PipelineLayout pipelineLayout;
        PipelineLayoutCreateInfo pipelineLayoutCreateInfo;
        std::vector<PipelineShaderStageCreateInfo> shaderStages;
        std::vector<VertexInputBindingDescription> vertexBindings;
        std::vector<VertexInputAttributeDescription> vertexAttributes;        
        std::vector<DynamicState> dynamicStates;
        std::vector<Viewport> viewports;
        std::vector<Scissor> scissors;
    };

    struct ShaderModuleData {
        std::vector<std::vector<char>> shaderCodes;
        std::vector<ShaderModule> shaderModules;
        std::vector<ShaderModuleCreateInfo> shaderModuleCreateInfos;
        std::vector<Flags::ShaderStage::Bits> shaderStages;
    };

    struct RenderPassData {
        RenderPass renderPass;
        RenderPassCreateInfo renderPassInfo;
        std::vector<Attachment> attachments;
        std::vector<Subpass> subpasses;
        std::vector<AttachmentReference> colorAttachmentRefs;
        AttachmentReference depthAttachmentRef;
        std::vector<SubpassDependency> subpassDependencies;

        SurfaceFormat surfaceFormat;
        PresentMode presentMode;
    };
    
    //class ShaderReflection {
    //public:
    //    std::vector<VertexInputBindingDescription> vertexBindings;
    //    std::vector<VertexInputAttributeDescription> vertexAttributes;
    //    std::vector<DescriptorSet::Layout::Binding> descriptorBindings;
    //    std::vector<PushConstantRange> pushConstants;
    //};
}