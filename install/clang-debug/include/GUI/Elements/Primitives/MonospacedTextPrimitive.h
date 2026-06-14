#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Instance.h"
#include "GUI/Elements/QuadElement.h"
#include "GUI/Rendering/Glypth.h"
#include "GUI/Elements/Primitives/LetterPrimitive.h"
#include "GUI/Rendering/Text.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
    // All the positions and dimensions are represented in Surface pixel coordinates
    class MonospacedTextPrimitive
    {
    protected:
        const Text* m_text;
        QuadPrimitive m_boundingBox;
        glm::ivec2 m_cellSize;
        bool m_drawInvisibleLetters = false;

    public:
        MonospacedTextPrimitive() = default;
        ~MonospacedTextPrimitive() = default;
        
        MonospacedTextPrimitive& setPosition(const glm::ivec2& position) { m_boundingBox.setPosition(position); return *this; }
        MonospacedTextPrimitive& setSize(const glm::ivec2& size) { m_boundingBox.setSize(size); return *this; }
        MonospacedTextPrimitive& setBackgroundTexture(TextureId texture) { m_boundingBox.setTexture(texture); return *this; }
        MonospacedTextPrimitive& setCellSize(const glm::ivec2& cellSize) { m_cellSize = cellSize; return *this; }
        MonospacedTextPrimitive& setText(const Text& text) { m_text = &text; return *this; }
        MonospacedTextPrimitive& setDrawInvisibleLetters(bool val) { m_drawInvisibleLetters = val; return *this; }

        const auto& getPosition() const { return m_boundingBox.getPosition(); }
        const auto& getSize() const { return m_boundingBox.getSize(); }
        auto getBackgroundTexture() const { return m_boundingBox.getTexture(); }
        const auto& getCellSize() const { return m_cellSize; }
        const auto& getText() const { return *m_text; }
        const auto& getDrawInvisibleLetters() const { return m_drawInvisibleLetters; }

        auto& getPosition() { return m_boundingBox.getPosition(); }
        auto& getSize() { return m_boundingBox.getSize(); }
        auto& getBackgroundTexture() { return m_boundingBox.getTexture(); }
        auto& getCellSize() { return m_cellSize; }
        auto& getDrawInvisibleLetters() { return m_drawInvisibleLetters; }
        
        void record(GUI::InstanceInterface& instance) {
            m_boundingBox.record(instance);

            glm::ivec2 cursor = glm::ivec2(0, 0);

            for(size_t lineIndex = 0; lineIndex < m_text->lineCount(); ++lineIndex) {
                cursor.x = 0;
                auto line = (*m_text)[lineIndex];
                if(cursor.y + m_cellSize.y > m_boundingBox.getSize().y) {
                    for(size_t i = 0; i < line.size(); ++i) {
                        if(cursor.x + m_cellSize.x > m_boundingBox.getSize().x) return;
                        if(!m_drawInvisibleLetters && static_cast<uint8_t>(line[i].utfCode) == '\n') continue;
                        Quad quad = makeQuad(line[i].glypth, cursor);
                        if(quad.position.y >= m_boundingBox.getSize().y + m_boundingBox.getPosition().y) continue;
                        size_t sizeNew = m_boundingBox.getSize().y + m_boundingBox.getPosition().y - quad.position.y;
                        quad.uvMax.y = sizeNew / static_cast<float>(quad.size.y);
                        quad.size.y = sizeNew;
                        instance.addQuad(quad);
                        cursor.x += m_cellSize.x;
                    }
                    return;
                }
                for(size_t i = 0; i < line.size(); ++i) {
                    if(cursor.x + m_cellSize.x > m_boundingBox.getSize().x) {
                        cursor.y += m_cellSize.y;
                        cursor.x = 0;
                        if(cursor.y + m_cellSize.y > m_boundingBox.getSize().y) {
                            for(; i < line.size(); ++i) {
                                if(cursor.x + m_cellSize.x > m_boundingBox.getSize().x) return;
                                if(!m_drawInvisibleLetters && static_cast<uint8_t>(line[i].utfCode) == '\n') continue;
                                Quad quad = makeQuad(line[i].glypth, cursor);
                                if(quad.position.y >= m_boundingBox.getSize().y + m_boundingBox.getPosition().y) continue;
                                size_t sizeNew = m_boundingBox.getSize().y + m_boundingBox.getPosition().y - quad.position.y;
                                quad.uvMax.y = sizeNew / static_cast<float>(quad.size.y);
                                quad.size.y = sizeNew;
                                instance.addQuad(quad);
                                cursor.x += m_cellSize.x;
                            }
                            return;
                        }
                    }
                    
                    if(!m_drawInvisibleLetters && static_cast<uint8_t>(line[i].utfCode) == '\n') continue;

                    instance.addQuad(makeQuad(line[i].glypth, cursor));
                    cursor.x += m_cellSize.x;
                }
                cursor.y += m_cellSize.y;
            }
        }

    private:

        Quad makeQuad(const Glypth* glypth, const glm::ivec2& cursor) const {
                Quad quad;
                quad.textureId = glypth->getTexture();
                quad.position = m_boundingBox.getPosition();
                quad.position += cursor;
                quad.size = glypth->getMetrics().size;
                quad.position.x += glypth->getMetrics().bearing.x;
                quad.position.y += m_cellSize.y - quad.size.y;                      // Allign to lower bound
                quad.position.y -= glypth->getMetrics().bearing.y - quad.size.y;    // Add vertical bearing

                quad.uvMin = glm::vec2(0, 0);
                quad.uvMax = glm::vec2(1, 1);

                return quad;
        }

    };
};