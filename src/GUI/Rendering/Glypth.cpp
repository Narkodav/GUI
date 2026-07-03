#include "GUI/Rendering/Glyph.h"
#include "GUI/Instance.h"

namespace GUI {

    void Glyph::create(GUI::Instance& instance, Metrics metrics, CharId charId, unsigned char* buffer, 
            uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp, Graphics::Format format) {
        const auto& ctx = instance.getRenderingContext();
        m_metrics = metrics;
        m_char = charId;

        if(width == 0 || height == 0) {
            m_texture = static_cast<TextureId>(DefaultTextureType::TransparentTexture);
            m_textureImage = nullptr;
            return;
        }

        Graphics::ImageCreateInfo imageInfo;

        imageInfo.setImageType(Graphics::ImageType::T2D)
            .setFormat(format)
            .setExtent({width, height, 1})
            .setMipLevels(1)
            .setArrayLayers(1)
            .setSamples(Graphics::Flags::SampleCount::Bits::SC1)
            .setTiling(Graphics::ImageTiling::Linear)
            .setUsage(Graphics::Flags::ImageUsage::Bits::Sampled)
            .setInitialLayout(Graphics::ImageLayout::Preinitialized);

        m_textureImage.create(*ctx.deviceFunctions, ctx.deviceRef, imageInfo);
        Graphics::MemoryRequirements memReq = m_textureImage.getMemoryRequirements(*ctx.deviceFunctions, ctx.deviceRef);

        // ImageSubresource& setAspectMask(Flags::ImageAspect aspectMask) { 
        //     this->aspectMask = aspectMask;
        // }
        // ImageSubresource& setMipLevel(uint32_t mipLevel) { 
        //     this->mipLevel = mipLevel;
        // }
        // ImageSubresource& setArrayLayer(uint32_t arrayLayer) { 
        //     this->arrayLayer = arrayLayer;
        // }
        Graphics::ImageSubresource subresource;
        subresource.setAspectMask(Graphics::Flags::ImageAspect::Bits::Color)
            .setArrayLayer(0).setMipLevel(0);

        auto subresourceLayout = m_textureImage.getSubresourceLayout(*ctx.deviceFunctions, ctx.deviceRef, subresource);

        Graphics::MemoryAllocateInfo alloc;

        auto memoryProps = ctx.physicalDevice.getMemoryProperties(*ctx.instanceFunctions);

        alloc.setAllocationSize(memReq.getSize())
            .setMemoryTypeIndex(Graphics::Utility::findMemoryTypeFirstFit(
                memoryProps, memReq.getMemoryTypeBits(), 
                Graphics::Flags::MemoryProperty::Bits::HostVisibleCoherent));

        m_textureMemory.create(*ctx.deviceFunctions, ctx.deviceRef, alloc);
        m_textureMemory.bindImage(*ctx.deviceFunctions, ctx.deviceRef, m_textureImage);

        auto mapping = m_textureMemory.map(*ctx.deviceFunctions, ctx.deviceRef);
        auto* data = mapping.get<uint8_t>();

        for(uint32_t y = 0; y < height; y++) {
            std::memcpy(
                data + y * subresourceLayout.getRowPitch(),
                buffer + y * pitch,
                pitch
            );
        }

        m_textureMemory.unmap(*ctx.deviceFunctions, ctx.deviceRef, mapping);

        Graphics::ImageViewCreateInfo viewInfo;

        viewInfo.setImage(m_textureImage)
            .setViewType(Graphics::ImageViewType::T2D)
            .setFormat(format)
            .getSubresourceRange()
            .setAspectMask(Graphics::Flags::ImageAspect::Bits::Color)
            .setLevelCount(1)
            .setLayerCount(1);
        if(format == Graphics::Format::R8Unorm) {
        viewInfo.getComponents()
            .setR(Graphics::ComponentSwizzle::Zero)
            .setG(Graphics::ComponentSwizzle::Zero)
            .setB(Graphics::ComponentSwizzle::Zero)
            .setA(Graphics::ComponentSwizzle::R);
        }
        else {
        viewInfo.getComponents()
            .setR(Graphics::ComponentSwizzle::R)
            .setG(Graphics::ComponentSwizzle::G)
            .setB(Graphics::ComponentSwizzle::B)
            .setA(Graphics::ComponentSwizzle::A);
        }

        m_textureView.create(*ctx.deviceFunctions, ctx.deviceRef, viewInfo);

        m_texture = instance.registerTexture(m_textureView);
    }

    void Glyph::destroy(GUI::Instance& instance) {
        const auto& ctx = instance.getRenderingContext();
        if(m_textureImage == nullptr) return;
        m_textureView.destroy(*ctx.deviceFunctions, ctx.deviceRef);
        m_textureImage.destroy(*ctx.deviceFunctions, ctx.deviceRef);
        m_textureMemory.destroy(*ctx.deviceFunctions, ctx.deviceRef);
    }

}