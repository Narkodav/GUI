#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Rendering/ShaderCache.h"
#include "GUI/Rendering/TextureDescriptor.h"
#include "GUI/Rendering/DefaultTextureCache.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>
#include <array>

namespace GUI {

    // size and position in pixel coordinates
    struct Quad
    {
        glm::ivec2 position;
        glm::ivec2 size;
        TextureId textureId;
    };

    class QuadRenderer
    {
    private:
        Graphics::Pipeline m_pipeline;

        // States
        Graphics::PipelineVertexInputStateCreateInfo m_vertexInputState;
        Graphics::PipelineInputAssemblyStateCreateInfo m_inputAssemblyState;
        Graphics::PipelineViewportStateCreateInfo m_viewportState;
        Graphics::PipelineRasterizationStateCreateInfo m_rasterizationState;
        Graphics::PipelineMultisampleStateCreateInfo m_msaaState;
        Graphics::PipelineColorBlendAttachmentState m_blendAttachmentState;
        Graphics::PipelineColorBlendStateCreateInfo m_blendState;
        std::array<Graphics::DynamicState, 2> m_dynamicStates;
        Graphics::PipelineDynamicStateCreateInfo m_dynamicState;
        Graphics::PipelineDepthStencilStateCreateInfo m_depthStencilState;
        Graphics::Format m_colorFormat;
        Graphics::PipelineRenderingCreateInfo m_renderingState;
        Graphics::PipelineCreateInfo m_pipelineCreateInfo;

        enum class BufferSpecialization {
            Vertex,
            Quad,
            Count
        };

        //Quad buffer        
        Graphics::MemoryAllocateInfo m_quadAllocInfo;
        Graphics::MemoryAllocateInfo m_vertexAllocInfo;
        Graphics::BufferCreateInfo m_quadBufferCreateInfo;
        Graphics::BufferCreateInfo m_vertexBufferCreateInfo;

        Graphics::MemoryRequirements m_quadMemoryRequirements;
        Graphics::MemoryRequirements m_vertexMemoryRequirements;

        Graphics::Memory m_quadMemory;
        Graphics::Memory m_vertexMemory;

        Graphics::MemoryMapping m_quadMapping;
        Graphics::MemoryMapping m_vertexMapping;

        std::array<Graphics::Buffer, 2> m_buffers;

        static const size_t s_quadBufferStartSize = 128;
        size_t m_quadCount;

        //vertex attributes
        struct Vertex {
            glm::vec2 position;
            glm::vec2 uv;
        };

        static inline const std::array<Vertex, 6> s_quadVertices = {
            Vertex{glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
            Vertex{glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},

            Vertex{glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
            Vertex{glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        };

        std::array<Graphics::VertexInputBindingDescription, 2> m_bindings;
        std::array<Graphics::VertexInputAttributeDescription, 5> m_attributes;

    public:

        void create(const Graphics::InstanceFunctionTable& instanceFunctions, const Graphics::DeviceFunctionTable& functions, 
            Graphics::DeviceRef device, Graphics::PhysicalDevice physicalDevice, Graphics::RenderPassRef renderPass, 
            std::span<const Graphics::PipelineShaderStageCreateInfo> cachedShaderStages, 
            Graphics::PipelineLayoutRef genericLayout) {

            // ------------------------------------------------------------
            // Shader Stages
            // ------------------------------------------------------------

            auto shaderStages = cachedShaderStages.subspan(static_cast<size_t>(ShaderCache::ShaderSpecialisation::QuadVert), 2);

            // ------------------------------------------------------------
            // Vertex Input
            // ------------------------------------------------------------

            m_bindings[0].setBinding(0).setStride(sizeof(Vertex)).setInputRate(Graphics::VertexInputRate::Vertex);
            m_bindings[1].setBinding(1).setStride(sizeof(Quad)).setInputRate(Graphics::VertexInputRate::Instance);

            m_attributes[0].setLocation(0).setBinding(0).setFormat(Graphics::Format::R32G32Sfloat).setOffset(0);
            m_attributes[1].setLocation(1).setBinding(0).setFormat(Graphics::Format::R32G32Sfloat).setOffset(sizeof(glm::vec2));

            m_attributes[2].setLocation(2).setBinding(1).setFormat(Graphics::Format::R32G32Sint).setOffset(0);
            m_attributes[3].setLocation(3).setBinding(1).setFormat(Graphics::Format::R32G32Sint).setOffset(sizeof(glm::ivec2));
            m_attributes[4].setLocation(4).setBinding(1).setFormat(Graphics::Format::R32Uint).setOffset(sizeof(glm::ivec2) * 2);

            m_vertexInputState.setVertexBindingDescriptions(m_bindings);
            m_vertexInputState.setVertexAttributeDescriptions(m_attributes);            

            // ------------------------------------------------------------
            // Input Assembly
            // ------------------------------------------------------------

            m_inputAssemblyState.setTopology(Graphics::PrimitiveTopology::TriangleList);

            // ------------------------------------------------------------
            // Viewport State
            // ------------------------------------------------------------

            m_viewportState.setViewportCount(1)
                .setScissorCount(1);

            // ------------------------------------------------------------
            // Rasterization
            // ------------------------------------------------------------

            m_rasterizationState.setPolygonMode(Graphics::PolygonMode::Fill)
                .setCullMode(Graphics::Flags::CullMode::Bits::None)
                .setFrontFace(Graphics::FrontFace::CounterClockwise)
                .setLineWidth(1);

            // ------------------------------------------------------------
            // Multisampling
            // ------------------------------------------------------------

            m_msaaState.setRasterizationSamples(Graphics::Flags::SampleCount::Bits::SC1);

            // ------------------------------------------------------------
            // Blending
            // ------------------------------------------------------------

            m_blendAttachmentState.setBlendEnable(true)
                .setSrcColorBlendFactor(Graphics::BlendFactor::SrcAlpha)
                .setDstColorBlendFactor(Graphics::BlendFactor::OneMinusSrcAlpha)
                .setColorBlendOp(Graphics::BlendOp::Add)            
                .setSrcAlphaBlendFactor(Graphics::BlendFactor::One)
                .setDstAlphaBlendFactor(Graphics::BlendFactor::OneMinusSrcAlpha)
                .setAlphaBlendOp(Graphics::BlendOp::Add)
                .setColorWriteMask(
                    Graphics::Flags::ColorComponent::Bits::R |
                    Graphics::Flags::ColorComponent::Bits::G |
                    Graphics::Flags::ColorComponent::Bits::B |
                    Graphics::Flags::ColorComponent::Bits::A);

            m_blendState.setAttachments(m_blendAttachmentState);

            // ------------------------------------------------------------
            // Dynamic States
            // ------------------------------------------------------------

            m_dynamicStates = {
                Graphics::DynamicState::Viewport,
                Graphics::DynamicState::Scissor
            };

            m_dynamicState.setDynamicStates(m_dynamicStates);

            // ------------------------------------------------------------
            // Depth State
            // ------------------------------------------------------------

            m_depthStencilState.setDepthTestEnable(false).setDepthWriteEnable(false);

            // ------------------------------------------------------------
            // Dynamic Rendering
            // ------------------------------------------------------------

            m_renderingState.setColorAttachmentFormats(m_colorFormat);

            // ------------------------------------------------------------
            // Graphics Pipeline
            // ------------------------------------------------------------

            m_pipelineCreateInfo.setNext(&m_renderingState)
                .setStages(shaderStages)
                .setVertexInputState(m_vertexInputState)
                .setInputAssemblyState(m_inputAssemblyState)
                .setViewportState(m_viewportState)
                .setRasterizationState(m_rasterizationState)

                .setMultisampleState(m_msaaState)
                .setDepthStencilState(m_depthStencilState)
                .setColorBlendState(m_blendState)
                .setDynamicState(m_dynamicState)

                .setLayout(genericLayout)
                .setRenderPass(renderPass);

            m_pipeline.create(functions, device, m_pipelineCreateInfo);

            auto memoryProps = physicalDevice.getMemoryProperties(instanceFunctions);

            createBuffers(functions, device, memoryProps);
        }

        void destroy(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device) {
            m_quadMapping.unmap(functions, device, m_quadMemory);
            m_vertexMapping.unmap(functions, device, m_vertexMemory);
            m_pipeline.destroy(functions, device);
            m_buffers[0].destroy(functions, device);
            m_buffers[1].destroy(functions, device);
            m_quadMemory.destroy(functions, device);
            m_vertexMemory.destroy(functions, device);
        }

        void updateQuads(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, std::span<const Quad> quads) {
            if(quads.size() * sizeof(Quad) > m_quadBufferCreateInfo.getSize()) growMemory(functions, device, quads.size() * sizeof(Quad) * 2);
            std::copy(quads.begin(), quads.end(), m_quadMapping.get<Quad>());
            m_quadCount = quads.size();
        }

        //Assumes cache descriptor set 0 is bound
        void record(const Graphics::DeviceFunctionTable& functions, Graphics::CommandBuffer cmd) {
            cmd.bindPipeline(functions, m_pipeline, Graphics::PipelineBindPoint::Graphics);
            std::array<Graphics::DeviceSize, 2> offsets = { 0, 0 };
            cmd.bindVertexBuffers(functions, 0, m_buffers, offsets);            
            cmd.draw(functions, 6, m_quadCount, 0, 0);
        }

    private:

        void createBuffers(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
            const Graphics::PhysicalDeviceMemoryProperties& deviceMemoryProps) {
            m_vertexBufferCreateInfo = { s_quadVertices.size() * sizeof(Vertex), 
                Graphics::Flags::BufferUsage::Bits::VertexBuffer, Graphics::SharingMode::Exclusive };
            m_quadBufferCreateInfo = { s_quadBufferStartSize * sizeof(Quad), 
                Graphics::Flags::BufferUsage::Bits::VertexBuffer, Graphics::SharingMode::Exclusive };

            auto& vertexBuffer = m_buffers[static_cast<size_t>(BufferSpecialization::Vertex)];
            auto& quadBuffer = m_buffers[static_cast<size_t>(BufferSpecialization::Quad)];

            vertexBuffer.create(functions, device, m_vertexBufferCreateInfo);
            m_vertexMemoryRequirements = vertexBuffer.getMemoryRequirements(functions, device);
            quadBuffer.create(functions, device, m_quadBufferCreateInfo);
            m_quadMemoryRequirements = quadBuffer.getMemoryRequirements(functions, device);

            m_vertexAllocInfo = { m_vertexMemoryRequirements.getSize(),
                Graphics::Utility::findMemoryTypeFirstFit(deviceMemoryProps, m_vertexMemoryRequirements.getMemoryTypeBits(), 
                Graphics::Flags::MemoryProperty::Bits::HostVisibleCoherent) };
            m_quadAllocInfo = { m_quadMemoryRequirements.getSize(),
                Graphics::Utility::findMemoryTypeFirstFit(deviceMemoryProps, m_quadMemoryRequirements.getMemoryTypeBits(), 
                Graphics::Flags::MemoryProperty::Bits::HostVisibleCoherent) };

            m_vertexMemory.create(functions, device, m_vertexAllocInfo);
            m_quadMemory.create(functions, device, m_quadAllocInfo);
            m_vertexMemory.bindBuffer(functions, device, vertexBuffer);
            m_quadMemory.bindBuffer(functions, device, quadBuffer);

            m_vertexMapping = m_vertexMemory.map(functions, device);
            m_quadMapping = m_quadMemory.map(functions, device);

            Vertex* vertices = m_vertexMapping.get<Vertex>();
            for(size_t i = 0; i < s_quadVertices.size(); ++i) vertices[i] = s_quadVertices[i];
        }

        void growMemory(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, size_t newSize) {
            m_quadBufferCreateInfo.setSize(newSize);

            auto& quadBuffer = m_buffers[static_cast<size_t>(BufferSpecialization::Quad)];
            quadBuffer.destroy(functions, device);
            m_quadMemory.destroy(functions, device);

            quadBuffer.create(functions, device, m_quadBufferCreateInfo);
            m_quadMemoryRequirements = quadBuffer.getMemoryRequirements(functions, device);

            m_quadAllocInfo.setAllocationSize(m_quadMemoryRequirements.getSize());

            m_quadMemory.create(functions, device, m_quadAllocInfo);
            m_quadMemory.bindBuffer(functions, device, quadBuffer);

            m_quadMapping = m_quadMemory.map(functions, device);
        }
    };
}