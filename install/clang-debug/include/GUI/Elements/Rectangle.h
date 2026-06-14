#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Elements/Box.h"
#include "GUI/Instance.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI::Elements
{
	class Rectangle : public Box {
	private:
        Quad m_quad = {
            glm::ivec2(0,0),
            glm::ivec2(0,0),
            glm::vec2(0,0),
            glm::vec2(1,1),
            static_cast<TextureId>(DefaultTextureType::WhiteTexture)
        };

	public:
        virtual Rectangle& setPosition(const glm::ivec2& position) override { m_quad.position = position; return *this; }
        virtual Rectangle& setSize(const glm::ivec2& dimensions) override { m_quad.size = dimensions; return *this; }

        virtual glm::ivec2 getPosition() const override { return m_quad.position; }
        virtual glm::ivec2 getSize() const override { return m_quad.size; }

        virtual Rectangle& setPositionX(int positionX) override { m_quad.position.x = positionX; return *this; }
        virtual Rectangle& setPositionY(int positionY) override { m_quad.position.y = positionY; return *this; }
        virtual Rectangle& setSizeX(int sizeX) override { m_quad.size.x = sizeX; return *this; }
        virtual Rectangle& setSizeY(int sizeY) override { m_quad.size.y = sizeY; return *this; }

        virtual int getPositionX() const override { return m_quad.position.x; }
        virtual int getPositionY() const override { return m_quad.position.y; }
        virtual int getSizeX() const override { return m_quad.size.x; }
        virtual int getSizeY() const override { return m_quad.size.y; }

		Rectangle& setUvMin(const glm::vec2& uvMin) { m_quad.uvMin = uvMin; return *this; }
        Rectangle& setUvMax(const glm::vec2& uvMax) { m_quad.uvMax = uvMax; return *this; }
        Rectangle& setTexture(TextureId texture) { m_quad.textureId = texture; return *this; }

        const glm::vec2& getUvMin() const { return m_quad.uvMin; }
        const glm::vec2& getUvMax() const { return m_quad.uvMax; }
        TextureId getTexture() const { return m_quad.textureId; }

		const Quad& getQuad() const { return m_quad; }

	protected:
		virtual bool onRecord(InstanceInterface& instance) const override {
			instance.addQuad(m_quad);
			return true;
		}
	};
}