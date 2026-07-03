#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Instance.h"
#include "GUI/Elements/Element.h"
#include "GUI/Properties/SizeConstraints.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI {

	class Border : public Element {
        GUI_DECLARE_OBJECT(Border, Element)
    private:
        glm::ivec2 m_position = {0, 0};
        glm::ivec2 m_size = {0, 0};
        glm::ivec2 m_offset = {0, 0};
        glm::ivec2 m_bounds = {0, 0};
        glm::ivec4 m_edgeWidths = {0, 0, 0, 0};
	public:
        enum class Region : uint8_t {
            CornerLeftTop,
            CornerRightTop,
            CornerLeftBottom,
            CornerRightBottom,
            EdgeTop,
            EdgeBottom,
            EdgeLeft,
            EdgeRight,
            Center,
            Count
        };

        Border() {
            addChild(std::make_unique<Rectangle>())->setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            addChild(std::make_unique<Rectangle>())->setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            addChild(std::make_unique<Rectangle>())->setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            addChild(std::make_unique<Rectangle>())->setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            addChild(std::make_unique<Rectangle>())->setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            addChild(std::make_unique<Rectangle>())->setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            addChild(std::make_unique<Rectangle>())->setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            addChild(std::make_unique<Rectangle>())->setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            addChild(std::make_unique<Rectangle>())->setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderCenterTexture));

            addProperty(std::make_unique<SizeConstraints>());
		}
        virtual ~Border() = default;

        template<ElementConcept T = Element>
        const T* getRegion(Region region) const { return getChild<T>(static_cast<size_t>(region)); }
        template<ElementConcept T = Element>
        T* getRegion(Region region) { return getChild<T>(static_cast<size_t>(region)); }

        template<ElementConcept U = Element, ElementConcept T>
        std::unique_ptr<U> setRegion(Region region, std::unique_ptr<T>&& elem) {
            auto priorChild = swapChild(static_cast<size_t>(region), std::move(elem));
            auto* r = getRegion(region);
            r->setPosition(priorChild->getPosition());
            r->setSize(priorChild->getSize());
            return priorChild;
        }

        virtual void onSetPosition(const glm::ivec2& position) override { m_position = position; }

        // Resizes the center
        virtual void onSetSize(const glm::ivec2& size) override { 
            m_size = size;
            auto centerSize = glm::ivec2(size.x - m_edgeWidths[static_cast<size_t>(Region::EdgeLeft) - 4] - 
                m_edgeWidths[static_cast<size_t>(Region::EdgeRight) - 4], size.y - m_edgeWidths[static_cast<size_t>(Region::EdgeTop) - 4] - 
                m_edgeWidths[static_cast<size_t>(Region::EdgeBottom) - 4]);
            getRegion(Region::EdgeLeft)->setSizeY(centerSize.y);
            getRegion(Region::EdgeRight)->setSizeY(centerSize.y);
            getRegion(Region::EdgeTop)->setSizeX(centerSize.x);
            getRegion(Region::EdgeBottom)->setSizeX(centerSize.x);
            getRegion(Region::Center)->setSize(centerSize);

            getRegion(Region::CornerLeftTop)->setPosition(glm::ivec2(0, 0));
            getRegion(Region::EdgeTop)->setPosition(glm::ivec2(m_edgeWidths[static_cast<size_t>(Region::EdgeLeft) - 4], 0));
            getRegion(Region::CornerRightTop)->setPosition(glm::ivec2(m_edgeWidths[static_cast<size_t>(Region::EdgeLeft) - 4] + centerSize.x, 0));

            getRegion(Region::EdgeLeft)->setPosition(glm::ivec2(0, m_edgeWidths[static_cast<size_t>(Region::EdgeTop) - 4]));
            getRegion(Region::Center)->setPosition(glm::ivec2(m_edgeWidths[static_cast<size_t>(Region::EdgeLeft) - 4], 
                m_edgeWidths[static_cast<size_t>(Region::EdgeTop) - 4]));
            getRegion(Region::EdgeRight)->setPosition(glm::ivec2(m_edgeWidths[static_cast<size_t>(Region::EdgeLeft) - 4] + centerSize.x, 
                m_edgeWidths[static_cast<size_t>(Region::EdgeTop) - 4]));

            getRegion(Region::CornerLeftBottom)->setPosition(glm::ivec2(0, m_edgeWidths[static_cast<size_t>(Region::EdgeTop) - 4] + centerSize.y));
            getRegion(Region::EdgeBottom)->setPosition(glm::ivec2(m_edgeWidths[static_cast<size_t>(Region::EdgeLeft) - 4], 
                m_edgeWidths[static_cast<size_t>(Region::EdgeTop) - 4] + centerSize.y));
            getRegion(Region::CornerRightBottom)->setPosition(glm::ivec2(m_edgeWidths[static_cast<size_t>(Region::EdgeLeft) - 4] + centerSize.x, 
                m_edgeWidths[static_cast<size_t>(Region::EdgeTop) - 4] + centerSize.y));
        }

        virtual void onSetPositionX(int positionX) override { m_position.x = positionX; }
        virtual void onSetPositionY(int positionY) override { m_position.y = positionY; }

        virtual void onSetSizeX(int sizeX) override { onSetSize(glm::ivec2(sizeX, getSizeY())); }
        virtual void onSetSizeY(int sizeY) override { onSetSize(glm::ivec2(getSizeX(), sizeY)); }

        Border& setLeftEdgeWidth(size_t width) {
            m_edgeWidths[static_cast<size_t>(Region::EdgeLeft) - 4] = width;

            getRegion(Region::CornerLeftTop)->setSizeX(width);
            getRegion(Region::CornerLeftBottom)->setSizeX(width);
            getRegion(Region::EdgeLeft)->setSizeX(width);

            getRegion(Region::EdgeTop)->setPositionX(width);
            getRegion(Region::Center)->setPositionX(width);
            getRegion(Region::EdgeBottom)->setPositionX(width);

            auto centerSizeX = getRegion(Region::Center)->getSizeX();
            getRegion(Region::CornerRightTop)->setPositionX(centerSizeX + width);
            getRegion(Region::EdgeRight)->setPositionX(centerSizeX + width);
            getRegion(Region::CornerRightBottom)->setPositionX(centerSizeX + width);

            getProperty<SizeConstraints>()->setMinSize(glm::ivec2(
                width + m_edgeWidths[static_cast<size_t>(Region::EdgeRight) - 4],
                m_edgeWidths[static_cast<size_t>(Region::EdgeTop) - 4] + m_edgeWidths[static_cast<size_t>(Region::EdgeBottom) - 4]
            ));

            return *this;
        }

        Border& setTopEdgeWidth(size_t width) {
            m_edgeWidths[static_cast<size_t>(Region::EdgeTop) - 4] = width;

            getRegion(Region::CornerLeftTop)->setSizeY(width);
            getRegion(Region::CornerRightTop)->setSizeY(width);
            getRegion(Region::EdgeTop)->setSizeY(width);

            getRegion(Region::EdgeLeft)->setPositionY(width);
            getRegion(Region::Center)->setPositionY(width);
            getRegion(Region::EdgeRight)->setPositionY(width);

            auto centerSizeY = getRegion(Region::Center)->getSizeY();
            getRegion(Region::CornerLeftBottom)->setPositionX(centerSizeY + width);
            getRegion(Region::EdgeBottom)->setPositionX(centerSizeY + width);
            getRegion(Region::CornerRightBottom)->setPositionX(centerSizeY + width);

            getProperty<SizeConstraints>()->setMinSize(glm::ivec2(
                m_edgeWidths[static_cast<size_t>(Region::EdgeLeft) - 4] + m_edgeWidths[static_cast<size_t>(Region::EdgeRight) - 4],
                width + m_edgeWidths[static_cast<size_t>(Region::EdgeBottom) - 4]
            ));

            return *this;
        }

        Border& setRightEdgeWidth(size_t width) {
            m_edgeWidths[static_cast<size_t>(Region::EdgeRight) - 4] = width;

            getRegion(Region::CornerRightTop)->setSizeX(width);
            getRegion(Region::CornerRightBottom)->setSizeX(width);
            getRegion(Region::EdgeRight)->setSizeX(width);

            getProperty<SizeConstraints>()->setMinSize(glm::ivec2(
                m_edgeWidths[static_cast<size_t>(Region::EdgeLeft) - 4] + width,
                m_edgeWidths[static_cast<size_t>(Region::EdgeTop) - 4] + m_edgeWidths[static_cast<size_t>(Region::EdgeBottom) - 4]
            ));
            
            return *this;
        }

        Border& setBottomEdgeWidth(size_t width) {
            m_edgeWidths[static_cast<size_t>(Region::EdgeBottom) - 4] = width;

            getRegion(Region::CornerLeftBottom)->setSizeY(width);
            getRegion(Region::CornerRightBottom)->setSizeY(width);
            getRegion(Region::EdgeBottom)->setSizeY(width);
            
            getProperty<SizeConstraints>()->setMinSize(glm::ivec2(
                m_edgeWidths[static_cast<size_t>(Region::EdgeLeft) - 4] + m_edgeWidths[static_cast<size_t>(Region::EdgeRight) - 4],
                m_edgeWidths[static_cast<size_t>(Region::EdgeTop) - 4] + width
            ));

            return *this;
        }

        virtual glm::ivec2 getPosition() const override { return m_position; }
        virtual glm::ivec2 getSize() const override { return m_size; }
        virtual int getPositionX() const override { return m_position.x; }
        virtual int getPositionY() const override { return m_position.y; }
        virtual int getSizeX() const override { return m_size.x; }
        virtual int getSizeY() const override { return m_size.y; }

        virtual glm::ivec2 getGlobalPosition() const override { return m_offset + m_position; }
		virtual int getGlobalPositionX() const override { return m_offset.x + m_position.x; }
		virtual int getGlobalPositionY() const override { return m_offset.y + m_position.y; }

        virtual glm::ivec2 getOffset() const override { return m_offset; }
		virtual int getOffsetX() const override { return m_offset.x; }
		virtual int getOffsetY() const override { return m_offset.y; }

        virtual glm::ivec2 getBounds() const override { return m_bounds; }
		virtual int getBoundsX() const override { return m_bounds.x; }
		virtual int getBoundsY() const override { return m_bounds.y; }

        virtual bool positionIsInside(const glm::ivec2& coord) const override {
            auto globalPosMin = m_offset + m_position;
            auto globalPosMax = globalPosMin + m_size;
            return coord.x >= globalPosMin.x && coord.y >= globalPosMin.y && 
                coord.x < globalPosMax.x && coord.y < globalPosMax.y;
        }

	protected:
        virtual bool onRecord(InstanceInterface& instance) const override {
            return true;
        }

		virtual Element& setOffset(const glm::ivec2& size) override {
            m_offset = size; return *this;
        }
		virtual Element& setBounds(const glm::ivec2& size) override {
            m_bounds = size; return *this;
        }

		//virtual bool onRecord(InstanceInterface&) const override { return true; }
    };
};