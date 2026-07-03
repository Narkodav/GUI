#pragma once
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftcolor.h>
#include <freetype/tttables.h>

#include "Graphics/Graphics.h"

#include "GUI/Rendering/Quad.h"
#include "GUI/Rendering/DefaultTextureCache.h"
#include "GUI/Rendering/TextureDescriptor.h"
#include "GUI/Elements/Element.h"
#include "GUI/Rendering/Font.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI {
    class FTInstance {
    private:
        FT_Library ft;

    public:
        FTInstance() = default;
        ~FTInstance() noexcept = default;

        FTInstance(const FTInstance&) = delete;
        FTInstance& operator=(const FTInstance&) = delete;

        FTInstance(FTInstance&&) = delete;
        FTInstance& operator=(FTInstance&&) = delete;

        void create() {
            if (FT_Init_FreeType(&ft))
                throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");

            int major;
            int minor;
            int patch;

            FT_Library_Version(ft, &major, &minor, &patch);

            std::cout << "FreeType version: " << major << "." << minor << "." << patch << std::endl;
        }

        void destroy() {
            FT_Done_FreeType(ft);
        }

        operator FT_Library() const {
            return ft;
        }
    };

    struct RenderingContext {
        Graphics::InstanceRef instanceRef;
        Graphics::DeviceRef deviceRef;
        Graphics::PhysicalDevice physicalDevice;
        const Graphics::InstanceFunctionTable* instanceFunctions;
        const Graphics::DeviceFunctionTable* deviceFunctions;
        Graphics::DescriptorPoolRef descriptorPoolRef;
        Graphics::RenderPassRef renderPassRef;
    };

    class Instance;

    class InstanceInterface {
        friend class Instance;
    private:
        Instance* m_instance;

        explicit InstanceInterface(Instance* instance) : m_instance(instance) {}

    public:
        InstanceInterface() = default;
        ~InstanceInterface() = default;

        InstanceInterface(const InstanceInterface&) = delete;
        InstanceInterface& operator=(const InstanceInterface&) = delete;

        InstanceInterface(InstanceInterface&&) = default;
        InstanceInterface& operator=(InstanceInterface&&) = default;

        void addQuad(const Quad& quad);
        const FTInstance& getFTInstance() const;
        const RenderingContext& getRenderingContext() const;
        TextureId registerTexture(Graphics::ImageViewRef view, 
            Graphics::ImageLayout layout = Graphics::ImageLayout::ShaderReadOnlyOptimal);

        Instance& getInstance() { return *m_instance; }
        
    };

    class Instance {
        friend class InstanceInterface;
    private:
        FTInstance m_ftInstance;

        RenderingContext m_renderingContext;

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

        void create(RenderingContext ctx) {
            m_renderingContext = ctx;

            m_ftInstance.create();

            m_shaders.create(*m_renderingContext.deviceFunctions, m_renderingContext.deviceRef);
            m_shaderStages = Graphics::Utility::createShaderStageInfos(m_shaders.getShaderModuleData());

            m_storageSetBindings[static_cast<size_t>(StorageSetBindingSpecialization::Texture)]
                .setBinding(0)
                .setDescriptorType(Graphics::DescriptorType::CombinedImageSampler)
                .setDescriptorCount(1024)
                .setStageFlags(Graphics::Flags::ShaderStage::Bits::Fragment);

            auto& storageSetLayoutInfo = m_setLayoutInfos[static_cast<size_t>(DescriptorSetSpecialization::Storage)];
            auto& storageSetLayout = m_setLayouts[static_cast<size_t>(DescriptorSetSpecialization::Storage)];
            storageSetLayoutInfo.setBindings(m_storageSetBindings);
            storageSetLayout.create(*m_renderingContext.deviceFunctions, m_renderingContext.deviceRef, storageSetLayoutInfo);

            Graphics::DescriptorSetAllocateInfo allocInfo;
            allocInfo.setDescriptorSetLayouts(m_setLayouts).setDescriptorPool(ctx.descriptorPoolRef);
            
            m_sets = ctx.descriptorPoolRef.allocateSets<static_cast<size_t>(DescriptorSetSpecialization::Count)>(
                *m_renderingContext.deviceFunctions, m_renderingContext.deviceRef, allocInfo);

            m_pushRange.setSize(sizeof(uint32_t) * 2).setOffset(0).setStageFlags(
                Graphics::Flags::ShaderStage::Bits::Vertex | Graphics::Flags::ShaderStage::Bits::Fragment);

            m_genericLayoutInfo.setSetLayouts(storageSetLayout).setPushConstantRanges(m_pushRange);
            m_genericLayout.create(*m_renderingContext.deviceFunctions, m_renderingContext.deviceRef, m_genericLayoutInfo);

            m_quadRenderer.create(*m_renderingContext.instanceFunctions, *m_renderingContext.deviceFunctions, m_renderingContext.deviceRef, 
                ctx.physicalDevice, ctx.renderPassRef, m_shaderStages, m_genericLayout);

            m_defaultTextureCache.create(*m_renderingContext.instanceFunctions, *m_renderingContext.deviceFunctions, m_renderingContext.deviceRef, ctx.physicalDevice);

            m_textureDescriptor.create(m_sets[static_cast<size_t>(DescriptorSetSpecialization::Storage)],
                static_cast<size_t>(StorageSetBindingSpecialization::Texture), 1024, 0);

            for(size_t i = 0; i < static_cast<size_t>(GUI::DefaultTextureType::Count); ++i) 
                m_textureDescriptor.registerTexture(*m_renderingContext.deviceFunctions, m_renderingContext.deviceRef, 
                    m_defaultTextureCache.getImage(i), m_defaultTextureCache.getSampler());
        }

        void destroy() {
            m_defaultTextureCache.destroy(*m_renderingContext.deviceFunctions, m_renderingContext.deviceRef);
            m_quadRenderer.destroy(*m_renderingContext.deviceFunctions, m_renderingContext.deviceRef);
            m_genericLayout.destroy(*m_renderingContext.deviceFunctions, m_renderingContext.deviceRef);
            for(auto& layout : m_setLayouts) layout.destroy(*m_renderingContext.deviceFunctions, m_renderingContext.deviceRef);
            m_shaders.destroy(*m_renderingContext.deviceFunctions, m_renderingContext.deviceRef);
            m_ftInstance.destroy();
        }

        void reset() {
            m_quads.clear();
        }

        void record(Element* root) {
            auto ctx = interface();
            root->record(ctx);
        }

        template<typename Container>
        void record(Container& roots) requires (std::convertible_to<typename Container::value_type, Element*>) {
            auto ctx = interface();
            for(auto* root : roots) root->record(ctx);
        }
        
        void record(std::initializer_list<Element*> roots) {
            auto ctx = interface();
            for(auto* root : roots) root->record(ctx);
        }

        void upload() {
            m_quadRenderer.updateQuads(*m_renderingContext.deviceFunctions, m_renderingContext.deviceRef, m_quads);
        }

        void render(Graphics::CommandBuffer cmd, const Graphics::Extent2D& frameBufferExtent) {
            std::array<uint32_t, 2> dimensions = { frameBufferExtent.getWidth(), frameBufferExtent.getHeight() };
            cmd.bindDescriptorSets(*m_renderingContext.deviceFunctions, Graphics::PipelineBindPoint::Graphics, m_genericLayout, 0, m_sets);
            cmd.pushConstants(*m_renderingContext.deviceFunctions, m_genericLayout, 
                Graphics::Flags::ShaderStage::Bits::Vertex | Graphics::Flags::ShaderStage::Bits::Fragment, 
                0, sizeof(uint32_t) * 2, dimensions.data());
            m_quadRenderer.render(*m_renderingContext.deviceFunctions, cmd);
        }

        TextureId registerTexture(Graphics::ImageViewRef view,
            Graphics::ImageLayout layout = Graphics::ImageLayout::ShaderReadOnlyOptimal) {
            return m_textureDescriptor.registerTexture(*m_renderingContext.deviceFunctions, m_renderingContext.deviceRef, view, m_defaultTextureCache.getSampler(), layout);
        };

        const FTInstance& getFTInstance() const { return m_ftInstance; };
        const RenderingContext& getRenderingContext() const { return m_renderingContext; }

    private:
        InstanceInterface interface() {  return InstanceInterface(this); }
    };
}