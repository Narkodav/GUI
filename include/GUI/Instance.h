#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Rendering/Quad.h"
#include "GUI/Rendering/DefaultTextureCache.h"
#include "GUI/Rendering/TextureDescriptor.h"
#include "GUI/Elements/Element.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{

    class Instance;

    class InstanceInterface {
        friend class Instance;
    private:
        Instance* m_instance;

        InstanceInterface(Instance* instance) : m_instance(instance) {}

    public:
        InstanceInterface() = default;
        ~InstanceInterface() = default;

        InstanceInterface(const InstanceInterface&) = delete;
        InstanceInterface& operator=(const InstanceInterface&) = delete;

        InstanceInterface(InstanceInterface&&) = default;
        InstanceInterface& operator=(InstanceInterface&&) = default;

        void addQuad(const Quad& quad);
    };

    class Instance
    {
        friend class InstanceInterface;
    private:

        // Shared layout
        Graphics::PipelineLayoutCreateInfo m_genericLayoutInfo;
        Graphics::PipelineLayout m_genericLayout;

        Graphics::PushConstantRange m_pushRange;

        GUI::QuadRenderer m_quadRenderer;
        std::vector<Quad> m_quads;

        GUI::DefaultTextureCache m_defaultTextureCache;
        GUI::TextureDescriptor m_textureDescriptor;

        GUI::ShaderCache m_shaders;
        std::vector<Graphics::PipelineShaderStageCreateInfo> m_shaderStages;

        enum class DescriptorSetSpecialization {
            Storage,
            Count
        };

        enum class StorageSetBindingSpecialization {
            Texture,
            Count
        };

        //Per set bindings
        std::array<Graphics::DescriptorSetLayoutBinding, static_cast<size_t>(StorageSetBindingSpecialization::Count)> m_storageSetBindings;

        //Shared descriptor sets
        std::array<Graphics::DescriptorSetLayoutCreateInfo, static_cast<size_t>(DescriptorSetSpecialization::Count)> m_setLayoutInfos;
        std::array<Graphics::DescriptorSetLayout, static_cast<size_t>(DescriptorSetSpecialization::Count)> m_setLayouts;
        std::array<Graphics::DescriptorSet, static_cast<size_t>(DescriptorSetSpecialization::Count)> m_sets;

    public:

        void create(const Graphics::InstanceFunctionTable& instanceFunctions, const Graphics::DeviceFunctionTable& functions, 
            Graphics::DeviceRef device, Graphics::PhysicalDevice physicalDevice, Graphics::DescriptorPoolRef descriptorPool,
            Graphics::RenderPassRef renderPass) {

            m_shaders.create(functions, device, "Shaders");
            m_shaderStages = Graphics::Utility::createShaderStageInfos(m_shaders.getShaderModuleData());

            m_storageSetBindings[static_cast<size_t>(StorageSetBindingSpecialization::Texture)]
                .setBinding(0)
                .setDescriptorType(Graphics::DescriptorType::CombinedImageSampler)
                .setDescriptorCount(1024)
                .setStageFlags(Graphics::Flags::ShaderStage::Bits::Fragment);

            auto& storgeSetLayoutInfo = m_setLayoutInfos[static_cast<size_t>(DescriptorSetSpecialization::Storage)];
            auto& storageSetLayout = m_setLayouts[static_cast<size_t>(DescriptorSetSpecialization::Storage)];
            storgeSetLayoutInfo.setBindings(m_storageSetBindings);
            storageSetLayout.create(functions, device, storgeSetLayoutInfo);

            Graphics::DescriptorSetAllocateInfo allocInfo;
            allocInfo.setDescriptorSetLayouts(m_setLayouts).setDescriptorPool(descriptorPool);
            
            m_sets = descriptorPool.allocateSets<static_cast<size_t>(DescriptorSetSpecialization::Count)>(functions, device, allocInfo);

            m_pushRange.setSize(sizeof(uint32_t) * 2).setOffset(0).setStageFlags(Graphics::Flags::ShaderStage::Bits::Vertex);

            m_genericLayoutInfo.setSetLayouts(storageSetLayout).setPushConstantRanges(m_pushRange);
            m_genericLayout.create(functions, device, m_genericLayoutInfo);

            m_quadRenderer.create(instanceFunctions, functions, device, physicalDevice, renderPass, m_shaderStages, m_genericLayout);

            m_defaultTextureCache.create(instanceFunctions, functions, device, physicalDevice);

            m_textureDescriptor.create(m_sets[static_cast<size_t>(DescriptorSetSpecialization::Storage)],
                static_cast<size_t>(StorageSetBindingSpecialization::Texture), 1024, 0);

            for(size_t i = 0; i < static_cast<size_t>(GUI::DefaultTextureCache::TextureType::Count); ++i) 
                m_textureDescriptor.registerTexture(functions, device, m_defaultTextureCache.getImage(i), m_defaultTextureCache.getSampler());
        }

        void destroy(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device) {
            m_defaultTextureCache.destroy(functions, device);
            m_quadRenderer.destroy(functions, device);
            m_genericLayout.destroy(functions, device);
            for(auto& layout : m_setLayouts) layout.destroy(functions, device);
            m_shaders.destroy(functions, device);
        }

        void record(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, Graphics::CommandBuffer cmd, 
            Element* root, const Graphics::Extent2D& frameBufferExtent) {
            m_quads.clear();
            InstanceInterface interface = this;
            root->record(interface);
            
            m_quadRenderer.updateQuads(functions, device, m_quads);

            // Viewport and scissor are bound by user
            std::array<uint32_t, 2> dimensions = { frameBufferExtent.getWidth(), frameBufferExtent.getHeight() };

            cmd.bindDescriptorSets(functions, Graphics::PipelineBindPoint::Graphics, m_genericLayout, 0, m_sets);
            cmd.pushConstants(functions, m_genericLayout, Graphics::Flags::ShaderStage::Bits::Vertex, 0, sizeof(uint32_t) * 2, dimensions.data());
            m_quadRenderer.record(functions, cmd);
        }
    };
}