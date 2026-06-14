#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Instance.h"
#include "GUI/Rendering/Text.h"
#include "GUI/Elements/Rectangle.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI::Elements
{
    // All the positions and dimensions are represented in Surface pixel coordinates
    class MonospacedText : public Rectangle {
    protected:
        Text m_text;
        bool m_drawInvisibleLetters = false;
        const Font::Size* m_size;

    public:
        MonospacedText() = default;
        ~MonospacedText() = default;

        MonospacedText& setText(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
            const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, Graphics::PhysicalDevice physicalDevice,
            const Font::Size& fontSize, std::string_view str) {
                m_size = &fontSize;
                m_text.setText(instance, instanceFunctions, functions, device, physicalDevice, fontSize, str); 
                return *this; 
        }
        MonospacedText& setDrawInvisibleLetters(bool val) { m_drawInvisibleLetters = val; return *this; }

        const auto& getText() const { return m_text; }
        const auto& getDrawInvisibleLetters() const { return m_drawInvisibleLetters; }
        const auto& getTextSize() const { return *m_size; }

        auto& getDrawInvisibleLetters() { return m_drawInvisibleLetters; }

	protected:
		virtual bool onRecord(InstanceInterface& instance) const override {
            Rectangle::onRecord(instance);

            glm::ivec2 cursor = glm::ivec2(0, 0);

            for(size_t lineIndex = 0; lineIndex < m_text.lineCount(); ++lineIndex) {
                cursor.x = 0;
                auto line = m_text[lineIndex];
                if(cursor.y + m_size->getMetrics().height > getSize().y) {
                    for(size_t i = 0; i < line.size(); ++i) {
                        if(cursor.x + m_size->getMetrics().maxAdvance > getSize().x) return true;
                        if(!m_drawInvisibleLetters && Text::letterIsInvisible(static_cast<uint8_t>(line[i].utfCode))) continue;
                        Quad quad = makeQuad(line[i].glyph, cursor);
                        if(quad.position.y >= getSize().y + getPosition().y) continue;
                        size_t sizeNew = getSize().y + getPosition().y - quad.position.y;
                        quad.uvMax.y = sizeNew / static_cast<float>(quad.size.y);
                        quad.size.y = sizeNew;
                        instance.addQuad(quad);
                        cursor.x += m_size->getMetrics().maxAdvance;
                    }
                    return true;
                }
                for(size_t i = 0; i < line.size(); ++i) {
                    if(cursor.x + m_size->getMetrics().maxAdvance > getSize().x) {
                        cursor.y += m_size->getMetrics().height;
                        cursor.x = 0;
                        if(cursor.y + m_size->getMetrics().height > getSize().y) {
                            for(; i < line.size(); ++i) {
                                if(cursor.x + m_size->getMetrics().maxAdvance > getSize().x) return true;
                                if(!m_drawInvisibleLetters && Text::letterIsInvisible(static_cast<uint8_t>(line[i].utfCode))) continue;
                                Quad quad = makeQuad(line[i].glyph, cursor);
                                if(quad.position.y >= getSize().y + getPosition().y) continue;
                                size_t sizeNew = getSize().y + getPosition().y - quad.position.y;
                                quad.uvMax.y = sizeNew / static_cast<float>(quad.size.y);
                                quad.size.y = sizeNew;
                                instance.addQuad(quad);
                                cursor.x += m_size->getMetrics().maxAdvance;
                            }
                            return true;
                        }
                    }
                    
                    if(!m_drawInvisibleLetters && Text::letterIsInvisible(static_cast<uint8_t>(line[i].utfCode))) continue;

                    instance.addQuad(makeQuad(line[i].glyph, cursor));
                    cursor.x += m_size->getMetrics().maxAdvance;
                }
                cursor.y += m_size->getMetrics().height;
            }
            return true;
        }

    private:
        Quad makeQuad(const Glyph* glyph, const glm::ivec2& cursor) const {
                Quad quad;
                quad.textureId = glyph->getTexture();
                quad.position = getPosition();
                quad.position += cursor;
                quad.size = glyph->getMetrics().size;
                quad.position.x += glyph->getMetrics().bearing.x;
                quad.position.y += m_size->getMetrics().ascender - quad.size.y;     // Align to lower bound
                quad.position.y -= glyph->getMetrics().bearing.y - quad.size.y;     // Add vertical bearing

                quad.uvMin = glm::vec2(0, 0);
                quad.uvMax = glm::vec2(1, 1);

                return quad;
        }
    };
};