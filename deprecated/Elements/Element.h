#pragma once
#include "Common.h"
#include "Window.h"

namespace GUI
{
    class Layer;

    class LayerInterface
    {
    public:
        LayerInterface() = default;
        virtual ~LayerInterface() = default;
        virtual void markDirty() = 0;
    };

    struct Color
    {
        float r;
        float g;
        float b;
        float a;
    };

    struct Rectangle
    {
        size_t width;
        size_t height;
    };

    //all the positions and dimensions are represented in NDC
    class Element
    {
    protected:
        LayerInterface* m_layerInterface = nullptr;

        // Position and dimensions in pixels, m_position is top left
        glm::ivec2 m_position;       //center of the element
        Rectangle m_dimensions;     

        // Hierarchy
        Element* m_parent{ nullptr };
        std::vector<std::unique_ptr<Element>> m_children;

        // State
        bool m_visible{ true };
        bool m_enabled{ true };
        bool m_focused{ false };
        bool m_hovered{ false };

    public:
        virtual ~Element() = default;

        //// Core methods
        //void update(float deltaTime) {
        //    for (auto& child : m_children) {
        //        child->update(deltaTime);
        //    }

        //    onUpdate(deltaTime);
        //}

        ////this is implemented by children and implements actual update logic
        //virtual void onUpdate(float deltaTime) = 0;

        virtual void render(const Extent2D& frameBufferDimensions) {
            if (!m_visible) return;

            // Render this element
            onRender(frameBufferDimensions);

            // Render children
            for (auto& child : m_children) {
                child->render(frameBufferDimensions);
            }
        }

        // Hierarchy management
        void addChild(std::unique_ptr<Element> child) {
            child->m_parent = this;
            m_children.push_back(std::move(child));
        }

        void removeChild(Element* child) {
            auto it = std::find_if(m_children.begin(), m_children.end(),
                [child](const auto& c) { return c.get() == child; });
            if (it != m_children.end()) {
                m_children.erase(it);
            }
        }

        virtual void setPosition(const glm::vec2& pos) {
            m_position = pos;
        }

        // these can actually be any size so if padding is smaller than border, the elements can overlap
        virtual void setSize(const Rectangle& border) {
            m_dimensions = border;
        }

        virtual bool cursorIsInside(glm::ivec2 cursorPos)
        {
            if (cursorPos.x > m_position.x && cursorPos.x < m_position.x + m_dimensions.width &&
                cursorPos.y > m_position.y && cursorPos.y < m_position.y + m_dimensions.height)
                return true;
            return false;
        }

        virtual void setLayer(LayerInterface* interface) {
            m_layerInterface = interface;
        }

        virtual void makeVisible() {
            m_visible = true;
            m_layerInterface->markDirty();
        }

        virtual void makeInvisible() {
            m_visible = false;
            m_layerInterface->markDirty();
        }

        virtual void makeDirty()
        {
            if (m_layerInterface != nullptr)
                m_layerInterface->markDirty();
        }

    protected:
        virtual void onRender(const Extent2D&) = 0;
        virtual void onRegistration(Window&) = 0; //here elements will register callbacks

        friend class Layer;
    };
};