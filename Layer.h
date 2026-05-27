#pragma once
#include "Common.h"
#include "Elements/Element.h"
#include "Window.h"

#include "Rendering/FrameBuffer.h"
#include "Shaders/ShaderCache.h"

namespace GUI
{
    class Layer : public LayerInterface
    {
    private:
        MT::EventSystem<WindowEventPolicy>::Subscription m_windowResizeSubscription;

        FrameBuffer m_frameBuffer;
        bool m_isDirty;  // Track if layer needs redraw
        std::vector<std::unique_ptr<Element>> m_elements;

        //coordinates are in NDC
        //glm::vec2 m_position;   // center from -1 to 1
        //glm::vec2 m_size;       // width and height from 0 to 2

        Extent2D m_windowSize; //in pixels
    public:

        Layer(Window& window);


        void addElement(std::unique_ptr<Element>&& element, Window& window) {
            m_elements.push_back(std::move(element));
            m_elements.back()->setLayer(this);
            m_elements.back()->onRegistration(window);
            markDirty();
        };

        void clear() { m_elements.clear(); };

        void render() {
            if (!m_isDirty) return;
                        
            m_frameBuffer.clear(glm::vec4(1.0f, 1.0f, 0.0f, 0.4f));
            m_frameBuffer.bind();

            for (auto& element : m_elements) {
                element->render(Extent2D(m_frameBuffer.getWidth(), m_frameBuffer.getHeight()));
            }

            m_frameBuffer.unbind();
            m_isDirty = false;
        };

        void draw()
        {
            m_frameBuffer.drawToScreen();
        }

    private:

        virtual void onWindowResize(int width, int height) {
            m_windowSize.width = width;
            m_windowSize.height = height;
            m_frameBuffer.resetProjection(width, height);
            markDirty();
        }

        virtual void markDirty() {
            m_isDirty = true;
        }
    };
}

