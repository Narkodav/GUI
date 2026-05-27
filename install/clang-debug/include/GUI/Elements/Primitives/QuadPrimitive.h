#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Instance.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
    // All the positions and dimensions are represented in Surface pixel coordinates
    class QuadPrimitive
    {
    protected:
        Quad m_quad = {
            glm::ivec2(0,0),
            glm::ivec2(0,0),
            static_cast<TextureId>(DefaultTextureCache::TextureType::WhiteTexture)
        };

    public:
        QuadPrimitive() = default;
        ~QuadPrimitive() = default;

        void setPosition(const glm::ivec2& position) { m_quad.position = position; }
        void setSize(const glm::ivec2& dimensions) { m_quad.size = dimensions; }
        void setTexture(TextureId texture) { m_quad.textureId = texture; }

        const glm::ivec2& getPosition() const { return m_quad.position; }
        const glm::ivec2& getSize() const { return m_quad.size; }
        TextureId getTexture() const { return m_quad.textureId; }

        void record(GUI::InstanceInterface& instance) {
            instance.addQuad(m_quad);
        }

        bool cursorIsInside(const Pointer& pointer) {
			const auto& pos = getPosition();
			const auto& size = getSize();
            if (pointer.position.x > pos.x && pointer.position.x < pos.x + size.x &&
                pointer.position.y > pos.y && pointer.position.y < pos.y + size.y)
                return true;
            return false;
        }
    };
};