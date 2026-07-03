#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Instance.h"
#include "GUI/Rendering/Text.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI {
    enum class HorizontalAlignment {
        Left, Right, Center, Count
    };
    enum class VerticalAlignment {
        Top, Bottom, Center, Count
    };
}

namespace GUI
{
    // All the positions and dimensions are represented in Surface pixel coordinates
    class Label : public Rectangle {
        GUI_DECLARE_OBJECT(Label, Rectangle)
    protected:
        Text m_text;
        bool m_drawInvisibleLetters = false;
        HorizontalAlignment m_alignmentH = HorizontalAlignment::Center;
        VerticalAlignment m_alignmentV = VerticalAlignment::Center;
        const Font::Size* m_size;

    public:
        ~Label() = default;

        Label& setText(GUI::Instance& instance, const Font::Size& fontSize, std::string_view str) {
                m_size = &fontSize;
                m_text.setText(instance, fontSize, str); 
                return *this; 
        }
        Label& setDrawInvisibleLetters(bool val) { m_drawInvisibleLetters = val; return *this; }

        const auto& getText() const { return m_text; }
        const auto& getDrawInvisibleLetters() const { return m_drawInvisibleLetters; }
        const auto& getTextSize() const { return *m_size; }

        Label& setHorizontalAlignment(HorizontalAlignment alignment) { m_alignmentH = alignment; return *this; }
        Label& setVerticalAlignment(VerticalAlignment alignment) { m_alignmentV = alignment; return *this; }

        const auto& getHorizontalAlignment() const { return m_alignmentH; }
        const auto& getVerticalAlignment() const { return m_alignmentV; }

	protected:
		virtual bool onRecord(InstanceInterface& instance) const override {
            Rectangle::onRecord(instance);
            if(m_size != nullptr) drawText(instance);
            return true;
        }

    private:
        Quad makeQuad(const Glyph* glyph, const glm::ivec2& cursor) const {
                Quad quad;
                quad.textureId = glyph->getTexture();
                quad.position = cursor;
                quad.size = glyph->getMetrics().size;
                quad.position.x += glyph->getMetrics().bearing.x;
                quad.position.y -= quad.size.y;                                     // Align to lower bound
                quad.position.y -= glyph->getMetrics().bearing.y - quad.size.y;     // Add vertical bearing

                quad.uvMin = glm::vec2(0, 0);
                quad.uvMax = glm::vec2(1, 1);

                quad.offset = getOffset() + getPosition();
                quad.bounds = getSize();

                return quad;
        }

        void drawText(InstanceInterface& instance) const {
            glm::ivec2 cursor(0, 0);
            size_t height = getSizeY();
            size_t textHeight = m_size->getMetrics().height * (m_text.lineCount() - 1) 
                + m_size->getMetrics().ascender + m_size->getMetrics().descender;
            size_t lineCount = height / m_size->getMetrics().height;
            
            switch(m_alignmentV) {
                case VerticalAlignment::Top:
                    cursor.y = 0;
                    break;
                case VerticalAlignment::Center:
                    cursor.y = height > textHeight ? (height - textHeight) / 2 + m_size->getMetrics().ascender : m_size->getMetrics().ascender;
                    break;
                case VerticalAlignment::Bottom:
                    cursor.y = height > textHeight ? height - textHeight + m_size->getMetrics().ascender : m_size->getMetrics().ascender;
                    break;
                default: break;
            }

            if(lineCount >= m_text.lineCount()) {
                for(size_t lineIndex = 0; lineIndex < m_text.lineCount(); ++lineIndex) {
                    drawLine(instance, cursor, lineIndex);
                    cursor.y += m_size->getMetrics().height;
                }
            }
            else {
                for(size_t lineIndex = 0; lineIndex < lineCount + 1; ++lineIndex) {
                    drawLine(instance, cursor, lineIndex);
                    cursor.y += m_size->getMetrics().height;
                }
            }
        }

        void drawLine(InstanceInterface& instance, glm::ivec2& cursor, size_t lineIndex) const {
            auto line = m_text[lineIndex];
            const auto& lineData = m_text.getLineData(lineIndex);
            size_t lineWidth = m_drawInvisibleLetters ? lineData.widthWithInvisible : lineData.width;
            size_t width = getSizeX();
            switch(m_alignmentH) {
                case HorizontalAlignment::Left:
                    cursor.x = 0;
                    break;
                case HorizontalAlignment::Center:
                    cursor.x = width > lineWidth ? (width - lineWidth) / 2 : 0;
                    break;
                case HorizontalAlignment::Right:
                    cursor.x = width > lineWidth ? width - lineWidth : 0;
                    break;
                default: break;
            }

            for(size_t i = 0; i < line.size(); ++i) {
                if(cursor.x + (line[i].glyph->getMetrics().advance.x >> 6) > width) {
                    if(!m_drawInvisibleLetters && Text::letterIsInvisible(static_cast<uint8_t>(line[i].utfCode))) return;
                    Quad quad = makeQuad(line[i].glyph, cursor);
                    if(quad.position.x >= width) return;
                    // size_t sizeNew = width + getPositionX() - quad.position.x;
                    // quad.uvMax.x = sizeNew / static_cast<float>(quad.size.x);
                    // quad.size.x = sizeNew;
                    instance.addQuad(quad);
                    return;
                }
                
                if(!m_drawInvisibleLetters && Text::letterIsInvisible(static_cast<uint8_t>(line[i].utfCode))) return;
                
                Quad quad = makeQuad(line[i].glyph, cursor);

                // if constexpr (isClipped) {
                //     if(quad.position.y >= getSizeY() + getPositionY()) continue;
                //     size_t sizeNew = getSizeY() + getPositionY() - quad.position.y;
                //     quad.uvMax.y = sizeNew / static_cast<float>(quad.size.y);
                //     quad.size.y = sizeNew;
                // }

                instance.addQuad(quad);
                cursor.x += line[i].glyph->getMetrics().advance.x >> 6;
            }
        }
    };
};