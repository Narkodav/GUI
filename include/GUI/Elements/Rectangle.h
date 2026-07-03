#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Elements/Element.h"
#include "GUI/Instance.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
	class Rectangle : public Element {
        GUI_DECLARE_OBJECT(Rectangle, Element)
	private:
        Quad m_quad = {
            glm::ivec2(0,0),
            glm::ivec2(0,0),
            glm::vec2(0,0),
            glm::vec2(1,1),
            static_cast<TextureId>(DefaultTextureType::WhiteTexture),
            glm::ivec2(0,0),
            glm::ivec2(std::numeric_limits<int>::max(), std::numeric_limits<int>::max())
        };

	public:
        virtual ~Rectangle() = default;

        virtual glm::ivec2 getPosition() const override { return m_quad.position; }
        virtual glm::ivec2 getSize() const override { return m_quad.size; }

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

        virtual bool positionIsInside(const glm::ivec2& coord) const override {
            auto globalPosMin = m_quad.offset + m_quad.position;
            auto globalPosMax = globalPosMin + m_quad.size;
            return coord.x >= globalPosMin.x && coord.y >= globalPosMin.y && 
                coord.x < globalPosMax.x && coord.y < globalPosMax.y;
        }

        virtual glm::ivec2 getGlobalPosition() const override {
            return m_quad.offset + m_quad.position;
        }
		virtual int getGlobalPositionX() const override {
            return m_quad.offset.x + m_quad.position.x;
        }
		virtual int getGlobalPositionY() const override {
            return m_quad.offset.y + m_quad.position.y;
        }

        virtual glm::ivec2 getOffset() const override {
            return m_quad.offset;
        }
		virtual int getOffsetX() const override {
            return m_quad.offset.x;
        }
		virtual int getOffsetY() const override {
            return m_quad.offset.y;
        }

        virtual glm::ivec2 getBounds() const override {
            return m_quad.bounds;
        }
		virtual int getBoundsX() const override {
            return m_quad.bounds.x;
        }
		virtual int getBoundsY() const override {
            return m_quad.bounds.y;
        }

	protected:

        virtual Element& setOffset(const glm::ivec2& size) override { m_quad.offset = size; return *this; }
		virtual Element& setBounds(const glm::ivec2& size) override { m_quad.bounds = size; return *this; }

		virtual bool onRecord(InstanceInterface& instance) const override {
            instance.addQuad(m_quad); return true;
        }

		virtual void onSetSize(const glm::ivec2& size) override { m_quad.size = size; }
		virtual void onSetSizeX(int sizeX) override { m_quad.size.x = sizeX; }
		virtual void onSetSizeY(int sizeY) override { m_quad.size.y = sizeY; }
		virtual void onSetPosition(const glm::ivec2& position) override { m_quad.position = position; }
		virtual void onSetPositionX(int positionX) override { m_quad.position.x = positionX; }
		virtual void onSetPositionY(int positionY) override { m_quad.position.y = positionY; }
	};
}