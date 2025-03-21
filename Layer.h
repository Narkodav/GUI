#pragma once
#include "Common.h"
#include "Element.h"
#include "Window.h"

#include "Rendering/FrameBuffer.h"

namespace GUI
{
    class Layer
    {
    private:
        enum class Shaders
        {
            FLAT_TO_SCREEN_PROJECTION,
            NUM,
        };

        static inline const std::array<std::string,
            static_cast<size_t>(Shaders::NUM)> shaderPaths = {
                "Shaders/FlatToScreenProjection.shader",
        };

        std::vector<MT::EventSystem<WindowEventPolicy>::Subscription> m_windowEventsSubs;
        std::vector<MT::EventSystem<IOEventPolicy>::Subscription> m_IOEventsSubs;

        FrameBuffer m_frameBuffer;
        bool m_isDirty;  // Track if layer needs redraw
        std::vector<std::unique_ptr<Element>> m_elements;

        //coordinates are in NDC
        glm::vec2 m_position;   // center from -1 to 1
        glm::vec2 m_size;       // width and height from 0 to 2

        Extent2D m_windowSize; //in pixels

        //each layer owns its own shaders
        std::array<Shader, static_cast<size_t>(Shaders::NUM)> m_shaders;
    public:

        Layer(const Window& window, glm::vec2 position = glm::vec2(0.0f),
            glm::vec2 size = glm::vec2(2.0f));


        void addElement(std::unique_ptr<Element>&& element) { m_elements.push_back(std::move(element)); };

        void clear() { m_elements.clear(); };

        void render(const Window& window) {
            if (!m_isDirty) return;
                        
            m_frameBuffer.clear();
            m_frameBuffer.bind();

            for (auto& element : m_elements) {
                element->render();
            }

            m_frameBuffer.unbind();

            m_isDirty = false;        
        };

        void draw()
        {
            Extent2D sizePixels = fromNDC(m_size, m_windowSize);
            Extent2D positionPixels = fromNDC(m_position, m_windowSize);
            positionPixels.width -= sizePixels.width / 2;
            positionPixels.height -= sizePixels.height / 2;

            m_frameBuffer.drawToScreen(positionPixels.width, positionPixels.height,
                sizePixels.width, sizePixels.height, m_windowSize.width, m_windowSize.height);
        }
    };
}

