#include "GUI/Rendering/Glypth.h"
#include "GUI/Instance.h"

namespace GUI {

    void Glypth::create(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
        const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, Graphics::PhysicalDevice physicalDevice,
        Metrics metrics, CharId charId, unsigned char* buffer, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp, Graphics::Format format) {

        m_metrics = metrics;
        m_char = charId;

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

        m_textureImage.create(functions, device, imageInfo);
        Graphics::MemoryRequirements memReq = m_textureImage.getMemoryRequirements(functions, device);

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

        auto subresourceLayout = m_textureImage.getSubresourceLayout(functions, device, subresource);

        Graphics::MemoryAllocateInfo alloc;

        auto memoryProps = physicalDevice.getMemoryProperties(instanceFunctions);

        alloc.setAllocationSize(memReq.getSize())
            .setMemoryTypeIndex(Graphics::Utility::findMemoryTypeFirstFit(
                memoryProps, memReq.getMemoryTypeBits(), 
                Graphics::Flags::MemoryProperty::Bits::HostVisibleCoherent));

        m_textureMemory.create(functions, device, alloc);
        m_textureMemory.bindImage(functions, device, m_textureImage);

        auto mapping = m_textureMemory.map(functions, device);
        auto* data = mapping.get<uint8_t>();

        for(uint32_t y = 0; y < height; y++) {
            std::memcpy(
                data + y * subresourceLayout.getRowPitch(),
                buffer + y * pitch,
                pitch
            );
        }

        m_textureMemory.unmap(functions, device, mapping);

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

        m_textureView.create(functions, device, viewInfo);

        m_texture = instance.registerTexture(functions, device, m_textureView);
    }

    void Glypth::destroy(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device) {
        m_textureView.destroy(functions, device);
        m_textureImage.destroy(functions, device);
        m_textureMemory.destroy(functions, device);
    }

}