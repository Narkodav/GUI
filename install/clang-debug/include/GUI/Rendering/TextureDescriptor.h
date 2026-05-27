#pragma once
#include "Graphics/Graphics.h"

#include <vector>
#include <queue>
#include <stdexcept>
#include <cstdint>

namespace GUI
{
    using TextureId = uint32_t;

    class TextureDescriptor
    {
    private:
        Graphics::DescriptorSet m_set;
        uint32_t m_descriptorBinding;
        std::vector<Graphics::DescriptorImageInfo> m_slots;
        uint32_t m_nextIndex;
        uint32_t m_startIndex;

    public:
        TextureDescriptor() = default;

        void create(
            Graphics::DescriptorSet set,
            uint32_t descriptorBinding,
            uint32_t maxTextures,
            uint32_t startIndex = 0)
        {
            m_set = set;
            m_descriptorBinding = descriptorBinding;
            m_nextIndex = 0;
            m_startIndex = startIndex;

            m_slots.clear();
            m_slots.resize(maxTextures);
        }

        [[nodiscard]]
        TextureId registerTexture(
            const Graphics::DeviceFunctionTable& functions,
            Graphics::DeviceRef device,
            Graphics::ImageViewRef view,
            Graphics::SamplerRef sampler,
            Graphics::ImageLayout layout = Graphics::ImageLayout::ShaderReadOnlyOptimal)
        {
            uint32_t index = m_nextIndex++;

            if (index >= m_slots.size())
            {
                throw std::runtime_error(
                    "TextureCache capacity exceeded");
            }

            auto& slot = m_slots[index];
            slot.setImageLayout(layout)
                .setImageView(view)
                .setSampler(sampler);

            writeDescriptor(functions, device, index, slot);

            return index + m_startIndex;
        }

        // Writes the whole thing to a new set or binding
        void migrate(
            const Graphics::DeviceFunctionTable& functions,
            Graphics::DeviceRef device,
            Graphics::DescriptorSet newSet,
            uint32_t newBinding) 
        {
            m_set = newSet;
            m_descriptorBinding = newBinding;
            Graphics::WriteDescriptorSet write;

            write.setDstSet(m_set)
                .setDstBinding(m_descriptorBinding)
                .setDstArrayElement(0)
                .setDescriptorType(Graphics::DescriptorType::CombinedImageSampler)
                .setImageInfo(m_slots);

            Graphics::DescriptorSet::update(functions, device, write);
        }

        size_t capacity() const { return m_slots.size(); }
        size_t size() const { return m_nextIndex; }

    private:
        void writeDescriptor(
            const Graphics::DeviceFunctionTable& functions,
            Graphics::DeviceRef device,
            uint32_t index,
            const Graphics::DescriptorImageInfo& slot)
        {
            Graphics::WriteDescriptorSet write;

            write.setDstSet(m_set)
                .setDstBinding(m_descriptorBinding)
                .setDstArrayElement(index)
                .setDescriptorType(Graphics::DescriptorType::CombinedImageSampler)
                .setImageInfo(slot);

            Graphics::DescriptorSet::update(functions, device, write);
        }
    };
}