#pragma once
#include "Graphics/Graphics.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
    class InstanceInterface;

    struct Pointer {
        glm::ivec2 position;        // UI Surface local pixel coordinates
        glm::vec2 scrollDelta;      // ScrollDelta if relevant
    };

    struct PointerEvent
    {
        enum class Type
        {
            Move,
            Press,
            Release,
            Scroll
        };

        Type type;
        Pointer pointer;
    };

    class Element
    {
    protected:
        Element* m_parent = nullptr;
        std::vector<Element*> m_children;

    public:
        virtual ~Element() = default;

        virtual void record(GUI::InstanceInterface& instance) {

            if(!onRecord(instance)) return;

            for (auto& child : m_children) {
                child->record(instance);
            }
        }

        void addChild(Element* child) {
            child->m_parent = this;
            m_children.push_back(child);
        }

        void removeChild(Element* child) {
            auto it = std::find_if(m_children.begin(), m_children.end(),
                [child](const auto& c) { return c == child; });
            if (it != m_children.end()) {
                m_children.erase(it);
            }
        }

        virtual void pointerEvent(const PointerEvent& event) {
            if(!onPointerEvent(event)) return;
            for (auto& child : m_children) {
                child->pointerEvent(event);
            }
        } 

    protected:
        virtual bool onRecord(InstanceInterface& instance) = 0;
        virtual bool onPointerEvent(const PointerEvent& event) = 0;
    };
};