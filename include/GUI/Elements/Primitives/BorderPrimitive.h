#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Instance.h"
#include "GUI/Elements/QuadElement.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
    class BorderPrimitive {
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

    protected:
        std::array<Quad, static_cast<size_t>(Region::Count)> m_quads = { 
            {
                glm::ivec2(0,0),
                glm::ivec2(0,0),
                glm::vec2(0,0),
                glm::vec2(1,1),
                static_cast<TextureId>(DefaultTextureType::DefaultBorderEdgeTexture)
            }, {
                glm::ivec2(0,0),
                glm::ivec2(0,0),
                glm::vec2(0,0),
                glm::vec2(1,1),
                static_cast<TextureId>(DefaultTextureType::DefaultBorderEdgeTexture)
            }, {
                glm::ivec2(0,0),
                glm::ivec2(0,0),
                glm::vec2(0,0),
                glm::vec2(1,1),
                static_cast<TextureId>(DefaultTextureType::DefaultBorderEdgeTexture)
            }, {
                glm::ivec2(0,0),
                glm::ivec2(0,0),
                glm::vec2(0,0),
                glm::vec2(1,1),
                static_cast<TextureId>(DefaultTextureType::DefaultBorderEdgeTexture)
            }, {
                glm::ivec2(0,0),
                glm::ivec2(0,0),
                glm::vec2(0,0),
                glm::vec2(1,1),
                static_cast<TextureId>(DefaultTextureType::DefaultBorderEdgeTexture)
            }, {
                glm::ivec2(0,0),
                glm::ivec2(0,0),
                glm::vec2(0,0),
                glm::vec2(1,1),
                static_cast<TextureId>(DefaultTextureType::DefaultBorderEdgeTexture)
            }, {
                glm::ivec2(0,0),
                glm::ivec2(0,0),
                glm::vec2(0,0),
                glm::vec2(1,1),
                static_cast<TextureId>(DefaultTextureType::DefaultBorderEdgeTexture)
            }, {
                glm::ivec2(0,0),
                glm::ivec2(0,0),
                glm::vec2(0,0),
                glm::vec2(1,1),
                static_cast<TextureId>(DefaultTextureType::DefaultBorderEdgeTexture)
            }, {
                glm::ivec2(0,0),
                glm::ivec2(0,0),
                glm::vec2(0,0),
                glm::vec2(1,1),
                static_cast<TextureId>(DefaultTextureType::DefaultBorderCenterTexture)
            },
        };

    public:
        BorderPrimitive() = default;
        ~BorderPrimitive() = default;

        BorderPrimitive& setPosition(const glm::ivec2& position) {
            glm::ivec2 diff = position - m_quads[static_cast<size_t>(Region::CornerLeftTop)].position;
            for(size_t i = 0; i < static_cast<size_t>(Region::Count); ++i) m_quads[i].position += diff;

            return *this;
        }

        BorderPrimitive& setCenterSize(const glm::ivec2& centerSize) {
            glm::ivec2 diff = centerSize - m_quads[static_cast<size_t>(Region::Center)].size;

            m_quads[static_cast<size_t>(Region::Center)].size = centerSize;

            m_quads[static_cast<size_t>(Region::EdgeTop)].size.x = centerSize.x;
            m_quads[static_cast<size_t>(Region::EdgeBottom)].size.x = centerSize.x;
            m_quads[static_cast<size_t>(Region::EdgeRight)].position.x += diff.x;
            m_quads[static_cast<size_t>(Region::CornerRightTop)].position.x += diff.x;
            m_quads[static_cast<size_t>(Region::CornerRightBottom)].position.x += diff.x;

            m_quads[static_cast<size_t>(Region::EdgeLeft)].size.y = centerSize.y;
            m_quads[static_cast<size_t>(Region::EdgeRight)].size.y = centerSize.y;
            m_quads[static_cast<size_t>(Region::EdgeBottom)].position.y += diff.y;
            m_quads[static_cast<size_t>(Region::CornerLeftBottom)].position.y += diff.y;
            m_quads[static_cast<size_t>(Region::CornerRightBottom)].position.y += diff.y;

            return *this;
        }

        BorderPrimitive& setLeftEdgeWidth(size_t width) {
            size_t diff = width - m_quads[static_cast<size_t>(Region::CornerLeftTop)].size.x;

            m_quads[static_cast<size_t>(Region::CornerLeftTop)].size.x = width;
            m_quads[static_cast<size_t>(Region::EdgeLeft)].size.x = width;
            m_quads[static_cast<size_t>(Region::CornerLeftBottom)].size.x = width;

            m_quads[static_cast<size_t>(Region::EdgeTop)].position.x += diff;
            m_quads[static_cast<size_t>(Region::Center)].position.x += diff;
            m_quads[static_cast<size_t>(Region::EdgeBottom)].position.x += diff;
            
            m_quads[static_cast<size_t>(Region::CornerRightTop)].position.x += diff;
            m_quads[static_cast<size_t>(Region::EdgeRight)].position.x += diff;
            m_quads[static_cast<size_t>(Region::CornerRightBottom)].position.x += diff;
            
            return *this;
        }

        BorderPrimitive& setTopEdgeWidth(size_t width) {
            size_t diff = width - m_quads[static_cast<size_t>(Region::CornerLeftTop)].size.y;

            m_quads[static_cast<size_t>(Region::CornerLeftTop)].size.y = width;
            m_quads[static_cast<size_t>(Region::EdgeTop)].size.y = width;
            m_quads[static_cast<size_t>(Region::CornerRightTop)].size.y = width;

            m_quads[static_cast<size_t>(Region::EdgeLeft)].position.y += diff;
            m_quads[static_cast<size_t>(Region::Center)].position.y += diff;
            m_quads[static_cast<size_t>(Region::EdgeRight)].position.y += diff;
            
            m_quads[static_cast<size_t>(Region::CornerLeftBottom)].position.y += diff;
            m_quads[static_cast<size_t>(Region::EdgeBottom)].position.y += diff;
            m_quads[static_cast<size_t>(Region::CornerRightBottom)].position.y += diff;
            
            return *this;
        }

        BorderPrimitive& setRightEdgeWidth(size_t width) {
            m_quads[static_cast<size_t>(Region::CornerRightTop)].size.x = width;
            m_quads[static_cast<size_t>(Region::EdgeRight)].size.x = width;
            m_quads[static_cast<size_t>(Region::CornerRightBottom)].size.x = width;
            return *this;
        }

        BorderPrimitive& setBottomEdgeWidth(size_t width) {
            m_quads[static_cast<size_t>(Region::CornerLeftBottom)].size.y = width;
            m_quads[static_cast<size_t>(Region::EdgeBottom)].size.y = width;
            m_quads[static_cast<size_t>(Region::CornerRightBottom)].size.y = width;
            return *this;
        }

        BorderPrimitive& setTexture(Region region, TextureId textureId) {
            m_quads[static_cast<size_t>(region)].textureId = textureId;
            return *this;
        }

        BorderPrimitive& setParameters(const glm::ivec2& position, const glm::ivec2& centerSize, 
            size_t leftEdgeWidth, size_t rightEdgeWidth, size_t topEdgeWidth, size_t bottomEdgeWidth) {
            glm::ivec2 centerPosition = position + glm::ivec2(leftEdgeWidth, topEdgeWidth);
            glm::ivec2 rightBottomPosition = centerPosition + centerSize;

            m_quads[static_cast<size_t>(Region::CornerLeftTop)].position = position;
            m_quads[static_cast<size_t>(Region::CornerLeftTop)].size = glm::ivec2(leftEdgeWidth, topEdgeWidth);

            m_quads[static_cast<size_t>(Region::EdgeTop)].position = glm::ivec2(centerPosition.x, position.y);
            m_quads[static_cast<size_t>(Region::EdgeTop)].size = glm::ivec2(centerSize.x, topEdgeWidth);

            m_quads[static_cast<size_t>(Region::CornerRightTop)].position = glm::ivec2(rightBottomPosition.x, position.y);
            m_quads[static_cast<size_t>(Region::CornerRightTop)].size = glm::ivec2(rightEdgeWidth, topEdgeWidth);


            m_quads[static_cast<size_t>(Region::EdgeLeft)].position = glm::ivec2(position.x, centerPosition.y);
            m_quads[static_cast<size_t>(Region::EdgeLeft)].size = glm::ivec2(leftEdgeWidth, centerSize.y);

            m_quads[static_cast<size_t>(Region::Center)].position = centerPosition;
            m_quads[static_cast<size_t>(Region::Center)].size = centerSize;

            m_quads[static_cast<size_t>(Region::EdgeRight)].position = glm::ivec2(rightBottomPosition.x, centerPosition.y);
            m_quads[static_cast<size_t>(Region::EdgeRight)].size = glm::ivec2(rightEdgeWidth, centerSize.y);

    
            m_quads[static_cast<size_t>(Region::CornerLeftBottom)].position = glm::ivec2(position.x, rightBottomPosition.y);
            m_quads[static_cast<size_t>(Region::CornerLeftBottom)].size = glm::ivec2(leftEdgeWidth, bottomEdgeWidth);

            m_quads[static_cast<size_t>(Region::EdgeBottom)].position = glm::ivec2(centerPosition.x, rightBottomPosition.y);
            m_quads[static_cast<size_t>(Region::EdgeBottom)].size = glm::ivec2(centerSize.x, bottomEdgeWidth);

            m_quads[static_cast<size_t>(Region::CornerRightBottom)].position = rightBottomPosition;
            m_quads[static_cast<size_t>(Region::CornerRightBottom)].size = glm::ivec2(rightEdgeWidth, centerSize.y);

            return *this;
        }

        void record(GUI::InstanceInterface& instance) {
            for(size_t i = 0; i < static_cast<size_t>(Region::Count); ++i) instance.addQuad(m_quads[i]);
        }
    };
};