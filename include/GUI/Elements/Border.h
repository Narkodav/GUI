#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Instance.h"
#include "GUI/Elements/Box.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI::Elements {

	class Border : public Box {
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

            // getRegion<Rectangle>(Region::CornerLeftTop)->       setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            // getRegion<Rectangle>(Region::CornerRightTop)->      setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            // getRegion<Rectangle>(Region::CornerLeftBottom)->    setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            // getRegion<Rectangle>(Region::CornerRightBottom)->   setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            // getRegion<Rectangle>(Region::EdgeTop)->             setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            // getRegion<Rectangle>(Region::EdgeBottom)->          setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            // getRegion<Rectangle>(Region::EdgeLeft)->            setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            // getRegion<Rectangle>(Region::EdgeRight)->           setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderEdgeTexture));
            // getRegion<Rectangle>(Region::Center)->              setTexture(static_cast<size_t>(DefaultTextureType::DefaultBorderCenterTexture));
		}
        virtual ~Border() = default;

        template<typename T = Box>
        const T* getRegion(Region region) const { return getChild<T>(static_cast<size_t>(region)); }
        template<typename T = Box>
        T* getRegion(Region region) { return getChild<T>(static_cast<size_t>(region)); }

        Border& setRegion(Region region, std::unique_ptr<Box>&& elem) {
            auto priorChild = swapChild(static_cast<size_t>(region), std::move(elem));
            auto* c = static_cast<Box*>(priorChild.get());
            auto* r = getRegion(region);
            r->setPosition(c->getPosition());
            r->setSize(c->getSize());
            return *this;
        }

        virtual Box& setPosition(const glm::ivec2& position) override {
            glm::ivec2 diff = position - getRegion(Region::CornerLeftTop)->getPosition();
            for(size_t i = 0; i < static_cast<size_t>(Region::Count); ++i) {
                auto* region = getRegion(static_cast<Region>(i));
                getRegion(static_cast<Region>(i))->setPosition(diff + region->getPosition());
            }
            return *this;
        }

        // Resizes the center
        virtual Box& setSize(const glm::ivec2& size) override {
            glm::ivec2 newCenterSize = size - getRegion(Region::CornerLeftTop)->getSize() - getRegion(Region::CornerRightBottom)->getSize();
            if(newCenterSize.x < 0) newCenterSize.x = 0;
            if(newCenterSize.y < 0) newCenterSize.y = 0;
            return setCenterSize(newCenterSize);
        }

        virtual Box& setPositionX(int positionX) override {
            int diff = positionX - getRegion(Region::CornerLeftTop)->getPositionX();
            for(size_t i = 0; i < static_cast<size_t>(Region::Count); ++i) {
                auto* region = getRegion(static_cast<Region>(i));
                region->setPositionX(region->getPositionX() + diff);
            }
            return *this;
        }

        virtual Box& setPositionY(int positionY) override {
            int diff = positionY - getRegion(Region::CornerLeftTop)->getPositionY();
            for(size_t i = 0; i < static_cast<size_t>(Region::Count); ++i) {
                auto* region = getRegion(static_cast<Region>(i));
                region->setPositionY(region->getPositionY() + diff);
            }
            return *this;
        }

        virtual Box& setSizeX(int sizeX) override {
            int newCenterSizeX = sizeX - getRegion(Region::CornerLeftTop)->getSizeX() - getRegion(Region::CornerRightBottom)->getSizeX();
            if(newCenterSizeX < 0) newCenterSizeX = 0;
            return setCenterSizeX(newCenterSizeX);
        }

        virtual Box& setSizeY(int sizeY) override {
            int newCenterSizeY = sizeY - getRegion(Region::CornerLeftTop)->getSizeY() - getRegion(Region::CornerRightBottom)->getSizeY();
                if(newCenterSizeY < 0) newCenterSizeY = 0;
            return setCenterSizeY(newCenterSizeY);
        }

        Border& setCenterSize(const glm::ivec2& centerSize) {
            glm::ivec2 diff = centerSize - getRegion(Region::Center)->getSize();

            getRegion(Region::Center)->setSize(centerSize);

            auto* edgeTop = getRegion(Region::EdgeTop);
            edgeTop->setSizeX(centerSize.x);

            auto* edgeLeft = getRegion(Region::EdgeLeft);
            edgeLeft->setSizeY(centerSize.y);

            auto* cornerRightTop = getRegion(Region::CornerRightTop);
            cornerRightTop->setPositionX(cornerRightTop->getPositionX() + diff.x);

            auto* cornerLeftBottom = getRegion(Region::CornerLeftBottom);
            cornerLeftBottom->setPositionY(cornerLeftBottom->getPositionY() + diff.y);

            auto* edgeBottom = getRegion(Region::EdgeBottom);
            edgeBottom->setSizeX(centerSize.x);
            edgeBottom->setPositionY(edgeBottom->getPositionY() + diff.y);

            auto* edgeRight = getRegion(Region::EdgeRight);
            edgeRight->setSizeY(centerSize.y);
            edgeRight->setPositionX(edgeRight->getPositionX() + diff.x);

            auto* cornerRightBottom = getRegion(Region::CornerRightBottom);
            cornerRightBottom->setPosition(cornerRightBottom->getPosition() + diff);

            return *this;
        }

        Border& setCenterSizeX(int centerSizeX) {
            int diff = centerSizeX - getRegion(Region::Center)->getSizeX();

            getRegion(Region::Center)->setSizeX(centerSizeX);

            auto* edgeTop = getRegion(Region::EdgeTop);
            edgeTop->setSizeX(centerSizeX);

            auto* cornerRightTop = getRegion(Region::CornerRightTop);
            cornerRightTop->setPositionX(cornerRightTop->getPositionX() + diff);

            auto* edgeBottom = getRegion(Region::EdgeBottom);
            edgeBottom->setSizeX(centerSizeX);

            auto* edgeRight = getRegion(Region::EdgeRight);
            edgeRight->setPositionX(edgeRight->getPositionX() + diff);

            auto* cornerRightBottom = getRegion(Region::CornerRightBottom);
            cornerRightBottom->setPositionX(cornerRightBottom->getPositionX() + diff);

            return *this;
        }

        Border& setCenterSizeY(int centerSizeY) {
            int diff = centerSizeY - getRegion(Region::Center)->getSizeY();

            getRegion(Region::Center)->setSizeY(centerSizeY);

            auto* edgeLeft = getRegion(Region::EdgeLeft);
            edgeLeft->setSizeY(centerSizeY);

            auto* cornerLeftBottom = getRegion(Region::CornerLeftBottom);
            cornerLeftBottom->setPositionY(cornerLeftBottom->getPositionY() + diff);

            auto* edgeBottom = getRegion(Region::EdgeBottom);
            edgeBottom->setPositionY(edgeBottom->getPositionY() + diff);

            auto* edgeRight = getRegion(Region::EdgeRight);
            edgeRight->setSizeY(centerSizeY);

            auto* cornerRightBottom = getRegion(Region::CornerRightBottom);
            cornerRightBottom->setPositionY(cornerRightBottom->getPositionY() + diff);

            return *this;
        }

        Border& setLeftEdgeWidth(size_t width) {
            size_t diff = width - getRegion(Region::CornerLeftTop)->getSizeX();

            getRegion(Region::CornerLeftTop)->setSizeX(width);
            getRegion(Region::EdgeLeft)->setSizeX(width);
            getRegion(Region::CornerLeftBottom)->setSizeX(width);

            getRegion(Region::EdgeTop)->setPositionX(getRegion(Region::EdgeTop)->getPositionX() + diff);
            getRegion(Region::Center)->setPositionX(getRegion(Region::Center)->getPositionX() + diff);
            getRegion(Region::EdgeBottom)->setPositionX(getRegion(Region::EdgeBottom)->getPositionX() + diff);

            getRegion(Region::CornerRightTop)->setPositionX(getRegion(Region::CornerRightTop)->getPositionX() + diff);
            getRegion(Region::EdgeRight)->setPositionX(getRegion(Region::EdgeRight)->getPositionX() + diff);
            getRegion(Region::CornerRightBottom)->setPositionX(getRegion(Region::CornerRightBottom)->getPositionX() + diff);
            
            return *this;
        }

        Border& setTopEdgeWidth(size_t width) {
            size_t diff = width - getRegion(Region::CornerLeftTop)->getSizeY();

            getRegion(Region::CornerLeftTop)->setSizeY(width);
            getRegion(Region::EdgeTop)->setSizeY(width);
            getRegion(Region::CornerRightTop)->setSizeY(width);

            getRegion(Region::EdgeLeft)->setPositionY(getRegion(Region::EdgeLeft)->getPositionY() + diff);
            getRegion(Region::Center)->setPositionY(getRegion(Region::Center)->getPositionY() + diff);
            getRegion(Region::EdgeRight)->setPositionY(getRegion(Region::EdgeRight)->getPositionY() + diff);

            getRegion(Region::CornerLeftBottom)->setPositionY(getRegion(Region::CornerLeftBottom)->getPositionY() + diff);
            getRegion(Region::EdgeBottom)->setPositionY(getRegion(Region::EdgeBottom)->getPositionY() + diff);
            getRegion(Region::CornerRightBottom)->setPositionY(getRegion(Region::CornerRightBottom)->getPositionY() + diff);
            
            return *this;
        }

        Border& setRightEdgeWidth(size_t width) {
            getRegion(Region::CornerRightTop)->setSizeX(width);
            getRegion(Region::EdgeRight)->setSizeX(width);
            getRegion(Region::CornerRightBottom)->setSizeX(width);
            return *this;
        }

        Border& setBottomEdgeWidth(size_t width) {
            getRegion(Region::CornerLeftBottom)->setSizeY(width);
            getRegion(Region::EdgeBottom)->setSizeY(width);
            getRegion(Region::CornerRightBottom)->setSizeY(width);
            return *this;
        }

        Border& setParameters(const glm::ivec2& position, const glm::ivec2& centerSize, 
            size_t leftEdgeWidth, size_t rightEdgeWidth, size_t topEdgeWidth, size_t bottomEdgeWidth) {
            glm::ivec2 centerPosition = position + glm::ivec2(leftEdgeWidth, topEdgeWidth);
            glm::ivec2 rightBottomPosition = centerPosition + centerSize;

            auto* cornerLeftTop = getRegion(Region::CornerLeftTop);
            cornerLeftTop->setPosition(position);
            cornerLeftTop->setSize(glm::ivec2(leftEdgeWidth, topEdgeWidth));

            auto* edgeTop = getRegion(Region::EdgeTop);
            edgeTop->setPosition(glm::ivec2(centerPosition.x, position.y));
            edgeTop->setSize(glm::ivec2(centerSize.x, topEdgeWidth));

            auto* cornerRightTop = getRegion(Region::CornerRightTop);
            cornerRightTop->setPosition(glm::ivec2(rightBottomPosition.x, position.y));
            cornerRightTop->setSize(glm::ivec2(rightEdgeWidth, topEdgeWidth));


            auto* edgeLeft = getRegion(Region::EdgeLeft);
            edgeLeft->setPosition(glm::ivec2(position.x, centerPosition.y));
            edgeLeft->setSize(glm::ivec2(leftEdgeWidth, centerSize.y));

            auto* center = getRegion(Region::Center);
            center->setPosition(centerPosition);
            center->setSize(centerSize);

            auto* edgeRight = getRegion(Region::EdgeRight);
            edgeRight->setPosition(glm::ivec2(rightBottomPosition.x, centerPosition.y));
            edgeRight->setSize(glm::ivec2(rightEdgeWidth, centerSize.y));


            auto* cornerLeftBottom = getRegion(Region::CornerLeftBottom);
            cornerLeftBottom->setPosition(glm::ivec2(position.x, rightBottomPosition.y));
            cornerLeftBottom->setSize(glm::ivec2(leftEdgeWidth, bottomEdgeWidth));

            auto* edgeBottom = getRegion(Region::EdgeBottom);
            edgeBottom->setPosition(glm::ivec2(centerPosition.x, rightBottomPosition.y));
            edgeBottom->setSize(glm::ivec2(centerSize.x, bottomEdgeWidth));

            auto* cornerRightBottom = getRegion(Region::CornerRightBottom);
            cornerRightBottom->setPosition(rightBottomPosition);
            cornerRightBottom->setSize(glm::ivec2(rightEdgeWidth, bottomEdgeWidth));

            return *this;
        }

        virtual glm::ivec2 getPosition() const override {
            return getRegion(Region::CornerLeftTop)->getPosition();
        }

        virtual glm::ivec2 getSize() const override {
            return getRegion(Region::CornerLeftTop)->getSize() 
                + getRegion(Region::Center)->getSize() 
                + getRegion(Region::CornerRightBottom)->getSize();
        }

        virtual int getPositionX() const override {
            return getRegion(Region::CornerLeftTop)->getPositionX();
        }

        virtual int getPositionY() const override {
            return getRegion(Region::CornerLeftTop)->getPositionY();
        }

        virtual int getSizeX() const override {
            return getRegion(Region::CornerLeftTop)->getSizeX() 
                + getRegion(Region::Center)->getSizeX() 
                + getRegion(Region::CornerRightBottom)->getSizeX();
        }

        virtual int getSizeY() const override {
            return getRegion(Region::CornerLeftTop)->getSizeY() 
                + getRegion(Region::Center)->getSizeY() 
                + getRegion(Region::CornerRightBottom)->getSizeY();
        }

	protected:
		virtual bool onRecord(InstanceInterface&) const override { return true; }
    };
};