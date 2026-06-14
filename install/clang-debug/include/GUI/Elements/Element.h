#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Capabilities/Capability.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
    class InstanceInterface;

    struct Pointer {
        glm::ivec2 position;        // UI Surface local pixel coordinates
        glm::vec2 scrollDelta;      // ScrollDelta if relevant
        bool pressed;
    };
}

namespace GUI::Elements
{
    class Element {
    private:
        Element* m_parent = nullptr;
        std::vector<std::unique_ptr<Element>> m_children;
        std::vector<std::unique_ptr<GUI::Capabilities::Capability>> m_capabilities;

    public:
        Element() = default;
        virtual ~Element() = default;

        Element(const Element&) = delete;
        Element(Element&&) = delete;
        Element& operator=(const Element&) = delete;
        Element& operator=(Element&&) = delete;

        template<typename T>
        T* addCapability(std::unique_ptr<T>&& capability) requires std::convertible_to<T*, GUI::Capabilities::Capability*> {
            m_capabilities.push_back(std::move(capability));
            m_capabilities.back().get()->attach(this);
            return static_cast<T*>(m_capabilities.back().get());
        }

        std::unique_ptr<GUI::Capabilities::Capability> removeCapability(GUI::Capabilities::Capability* capability) {
            auto it = std::find_if(m_capabilities.begin(), m_capabilities.end(),
                [capability](const auto& c) { return c.get() == capability; });
            if (it != m_capabilities.end()) {
                auto ptr = std::move(*it);
                m_capabilities.erase(it);
                return ptr;
            }
            return nullptr;
        }

        size_t getCapabilityCount() const { return m_capabilities.size(); }

        template<typename T>
        T* getCapability() {
            for (auto& capability : m_capabilities) {
                if (auto* pt = dynamic_cast<T*>(capability.get())) return pt;
            }
            return nullptr;
        }

        template<typename T>
        const T* getCapability() const {
            for (const auto& capability : m_capabilities) {
                if (const auto* pt = dynamic_cast<const T*>(capability.get())) return pt;
            }
            return nullptr;
        }

        template<typename T = GUI::Capabilities::Capability>
        T* getCapability(size_t index) { return static_cast<T*>(m_capabilities[index].get()); }

        template<typename T = GUI::Capabilities::Capability>
        const T* getCapability(size_t index) const { return static_cast<const T*>(m_capabilities[index].get()); }

        template<typename T>
        T* addChild(std::unique_ptr<T>&& child) requires std::convertible_to<T*, Element*> {
            child->m_parent = this;
            m_children.push_back(std::move(child));
            return static_cast<T*>(m_children.back().get());
        }

        std::unique_ptr<Element> removeChild(Element* child) {
            auto it = std::find_if(m_children.begin(), m_children.end(),
                [child](const auto& c) { return c.get() == child; });
            if (it != m_children.end()) {
                auto ptr = std::move(*it);
                m_children.erase(it);
                return ptr;
            }
            return nullptr;
        }

        size_t getChildCount() const { return m_children.size(); }

        template<typename T = Element>
        T* getChild(size_t index) { return static_cast<T*>(m_children[index].get()); }

        template<typename T = Element>
        const T* getChild(size_t index) const { return static_cast<const T*>(m_children[index].get()); }

        template<typename T = Element>
        T* getParent() { return static_cast<T*>(m_parent); }

        template<typename T = Element>
        const T* getParent() const { return static_cast<const T*>(m_parent); }

        std::unique_ptr<Element> swapChild(size_t index, std::unique_ptr<Element>&& child) {
            child->m_parent = this;
            return std::exchange(m_children[index], std::move(child));
        }

        const Element& record(GUI::InstanceInterface& instance) const {
            if(!onRecord(instance)) return *this;
            for (const auto& child : m_children) child->record(instance);
            return *this;
        }

    protected:
        virtual bool onRecord(InstanceInterface& instance) const = 0;
    };
};