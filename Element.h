#pragma once
#include "Common.h"

namespace GUI
{

    struct Color
    {
        float r;
        float g;
        float b;
        float a;
    };

    struct Rectangle
    {
        float left;
        float right;
        float top;
        float bottom;
    };

    //all the positions and dimensions are represented in NDC
    class Element
    {
    protected:
        // Position and dimensions
        glm::vec2 m_position;
        glm::vec2 m_size;
        glm::vec2 m_scale{ 1.0f, 1.0f };
        float m_rotation{ 0.0f };

        // Hierarchy
        Element* m_parent{ nullptr };
        std::vector<std::unique_ptr<Element>> m_children;

        // State
        bool m_visible{ true };
        bool m_enabled{ true };
        bool m_focused{ false };
        bool m_hovered{ false };

        // Appearance
        Color m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
        float m_opacity{ 1.0f };
        Rectangle m_padding;
        Rectangle m_margin;

        // Layout
        enum class Anchor {
            TopLeft, TopCenter, TopRight,
            MiddleLeft, Center, MiddleRight,
            BottomLeft, BottomCenter, BottomRight
        };
        Anchor m_anchor{ Anchor::TopLeft };

    public:
        // Core methods
        void update(float deltaTime) {
            for (auto& child : m_children) {
                child->update(deltaTime);
            }

            onUpdate(deltaTime);
        }

        //this is implemented by children and implements actual update logic
        virtual void onUpdate(float deltaTime) = 0;

        virtual void render() {
            if (!m_visible) return;

            // Render this element
            onRender();

            // Render children
            for (auto& child : m_children) {
                child->render();
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

        virtual void setSize(const glm::vec2& newSize) {
            m_size = newSize;
            onResize();
        }

        // State management
        virtual void show() { m_visible = true; }
        virtual void hide() { m_visible = false; }
        virtual void enable() { m_enabled = true; }
        virtual void disable() { m_enabled = false; }

        // Event handlers
        virtual void onFocusGained() { m_focused = true; }
        virtual void onFocusLost() { m_focused = false; }
        virtual void onHoverEnter() { m_hovered = true; }
        virtual void onHoverExit() { m_hovered = false; }

        // Layout methods
        virtual void layout() {
            updateLayout();
            for (auto& child : m_children) {
                child->layout();
            }
        }

    protected:
        virtual void onRender() = 0;
        //virtual bool onInput(const InputEvent& event) { return false; }
        virtual void onResize() {}

        void updateLayout() {
            // Calculate position based on anchor and parent size
            if (m_parent) {
                //glm::vec2 parentSize = parent->getSize();
                //switch (anchor) {
                //case Anchor::Center:
                //    position.x = (parentSize.x - size.x) / 2;
                //    position.y = (parentSize.y - size.y) / 2;
                //    break;
                //    // ... other anchor calculations
                //}
            }

        }
    };
};