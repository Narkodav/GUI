#pragma once
#include "GUI/Instance.h"
#include "GUI/Elements/Element.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI::Elements
{
	class Box : public Element {
	public:
        virtual Box& setPosition(const glm::ivec2& position) = 0;
        virtual Box& setSize(const glm::ivec2& size) = 0;

        virtual Box& setPositionX(int positionX) = 0;
        virtual Box& setPositionY(int positionY) = 0;
        virtual Box& setSizeX(int sizeX) = 0;
        virtual Box& setSizeY(int sizeY) = 0;

        virtual glm::ivec2 getPosition() const = 0;
        virtual glm::ivec2 getSize() const = 0;

        virtual int getPositionX() const = 0;
        virtual int getPositionY() const = 0;
        virtual int getSizeX() const = 0;
        virtual int getSizeY() const = 0;
	};
}