#pragma once
#include "GUI/Rendering/TextureDescriptor.h"

#include <array>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <cstdint>

namespace GUI
{
    class DefaultTextureCache
    {
    public:
        enum class TextureType : TextureId
        {
            WhiteTexture        = 0,
            BlackTexture        = 1,
            TransparentTexture  = 2,
            CheckerTexture      = 3,
            Count               = 4,
            FirstUserTexture    = Count
        };

    private:
        struct TextureEntry
        {
            Graphics::Image image = nullptr;
            Graphics::ImageView view = nullptr;
            Graphics::Memory memory = nullptr;
        };

        std::array<TextureEntry, static_cast<size_t>(TextureType::Count)> m_textures;
        Graphics::Sampler m_defaultSampler;

    public:
        DefaultTextureCache() = default;

        void create(
            const Graphics::InstanceFunctionTable& instanceFunctions,
            const Graphics::DeviceFunctionTable& functions, 
            Graphics::DeviceRef device,
            Graphics::PhysicalDevice physicalDevice)
        {
            createDefaultTextures(
                instanceFunctions,
                functions,
                device,
                physicalDevice);
            
            m_defaultSampler.create(functions, device, Graphics::SamplerCreateInfo::uiSampler());
        }

        void destroy(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device)
        {
            for (TextureEntry& tex : m_textures) {
                tex.view.destroy(functions, device);
                tex.image.destroy(functions, device);
                tex.memory.destroy(functions, device);
            }
            m_defaultSampler.destroy(functions, device);
        }

        Graphics::ImageViewRef getImage(TextureType type) {
            return m_textures[static_cast<size_t>(type)].view;
        }

        Graphics::ImageViewRef getImage(size_t type) {
            return m_textures[type].view;
        }

        Graphics::SamplerRef getSampler() { return m_defaultSampler; }

    private:

        void createDefaultTextures(
            const Graphics::InstanceFunctionTable& instanceFunctions,
            const Graphics::DeviceFunctionTable& functions, 
            Graphics::DeviceRef device,
            Graphics::PhysicalDevice physicalDevice)
        {
            m_textures[0] = createSolidTexture(
                instanceFunctions,
                functions,
                device,
                physicalDevice,
                255, 255, 255, 255);

            m_textures[1] = createSolidTexture(
                instanceFunctions,
                functions,
                device,
                physicalDevice,
                0, 0, 0, 255);

            m_textures[2] = createSolidTexture(
                instanceFunctions,
                functions,
                device,
                physicalDevice,
                0, 0, 0, 0);

            m_textures[3] = createCheckerTexture(
                instanceFunctions,
                functions,
                device,
                physicalDevice);
        }

        TextureEntry createSolidTexture(
            const Graphics::InstanceFunctionTable& instanceFunctions,
            const Graphics::DeviceFunctionTable& functions, 
            Graphics::DeviceRef device,
            Graphics::PhysicalDevice physicalDevice,
            uint8_t r,
            uint8_t g,
            uint8_t b,
            uint8_t a)
        {
            std::array<uint8_t, 4> pixels{
                r, g, b, a
            };

            return createTexture(
                instanceFunctions,
                functions,
                device,
                physicalDevice,
                1,
                1,
                pixels.data());
        }

        TextureEntry createCheckerTexture(
            const Graphics::InstanceFunctionTable& instanceFunctions,
            const Graphics::DeviceFunctionTable& functions, 
            Graphics::DeviceRef device,
            Graphics::PhysicalDevice physicalDevice)
        {
            constexpr uint32_t size = 4;

            std::array<uint8_t, size * size * 4> pixels{};

            for (uint32_t y = 0; y < size; ++y)
            {
                for (uint32_t x = 0; x < size; ++x)
                {
                    const bool white =
                        ((x + y) & 1) == 0;

                    const uint8_t value =
                        white ? 255 : 80;

                    const uint32_t i =
                        (y * size + x) * 4;

                    pixels[i + 0] = value;
                    pixels[i + 1] = value;
                    pixels[i + 2] = value;
                    pixels[i + 3] = 255;
                }
            }

            return createTexture(
                instanceFunctions,
                functions,
                device,
                physicalDevice,
                size,
                size,
                pixels.data());
        }

        TextureEntry createTexture(
            const Graphics::InstanceFunctionTable& instanceFunctions,
            const Graphics::DeviceFunctionTable& functions, 
            Graphics::DeviceRef device,
            Graphics::PhysicalDevice physicalDevice,
            uint32_t width,
            uint32_t height,
            const void* pixelData)
        {
            TextureEntry texture;

            Graphics::ImageCreateInfo imageInfo;

            imageInfo.setImageType(Graphics::ImageType::T2D)
                .setFormat(Graphics::Format::R8G8B8A8Unorm)
                .setExtent({width, height, 1})
                .setMipLevels(1)
                .setArrayLayers(1)
                .setSamples(Graphics::Flags::SampleCount::Bits::SC1)
                .setTiling(Graphics::ImageTiling::Linear)
                .setUsage(Graphics::Flags::ImageUsage::Bits::Sampled)
                .setInitialLayout(Graphics::ImageLayout::Preinitialized);

            texture.image.create(functions, device, imageInfo);
            Graphics::MemoryRequirements memReq = texture.image.getMemoryRequirements(functions, device);

            Graphics::MemoryAllocateInfo alloc;

            auto memoryProps = physicalDevice.getMemoryProperties(instanceFunctions);

            alloc.setAllocationSize(memReq.getSize())
                .setMemoryTypeIndex(Graphics::Utility::findMemoryTypeFirstFit(
                    memoryProps, memReq.getMemoryTypeBits(), 
                    Graphics::Flags::MemoryProperty::Bits::HostVisibleCoherent));

            texture.memory.create(functions, device, alloc);
            texture.memory.bindImage(functions, device, texture.image);

            auto mapping = texture.memory.map(functions, device);
            auto* data = mapping.get();

            std::memcpy(data, pixelData, width * height * 4);

            texture.memory.unmap(functions, device, mapping);

            Graphics::ImageViewCreateInfo viewInfo;

            viewInfo.setImage(texture.image)
                .setViewType(Graphics::ImageViewType::T2D)
                .setFormat(Graphics::Format::R8G8B8A8Unorm)
                .getSubresourceRange()
                .setAspectMask(Graphics::Flags::ImageAspect::Bits::Color)
                .setLevelCount(1)
                .setLayerCount(1);

            texture.view.create(functions, device, viewInfo);

            return texture;
        }
    };
}