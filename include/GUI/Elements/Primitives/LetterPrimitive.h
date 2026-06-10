#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Instance.h"
#include "GUI/Elements/QuadElement.h"
#include "GUI/Rendering/Glypth.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
    // All the positions and dimensions are represented in pixel coordinates
    class LetterPrimitive
    {
    protected:
        QuadPrimitive m_quad;
        const Glypth* m_glypth;

    public:
        LetterPrimitive() = default;
        ~LetterPrimitive() = default;

        void setPosition(const glm::ivec2& position) { m_quad.setPosition(position); }
        void setGlypth(const Glypth& glypth) { return setGlypth(&glypth); }
        void setGlypth(const Glypth* glypth) {
            m_glypth = glypth;
            m_quad.setTexture(m_glypth->getTexture());
            m_quad.setSize(m_glypth->getMetrics().size);
        }

        const QuadPrimitive& getQuad() const { return m_quad; }
        const Glypth& getGlypth() const { return *m_glypth; }

        void record(GUI::InstanceInterface& instance) {
            m_quad.record(instance);
        }
    };
};