#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI::Elements
{
    class Element;
}

namespace GUI::Capabilities
{
    class Capability {
    private:
        GUI::Elements::Element* m_parent = nullptr;

    public:
        Capability() = default;
        virtual ~Capability() = default;

        Capability(const Capability&) = delete;
        Capability(Capability&&) = delete;
        Capability& operator=(const Capability&) = delete;
        Capability& operator=(Capability&&) = delete;

        void attach(GUI::Elements::Element* parent) {
            m_parent = parent;
            onAttach();
        }

        template<typename T = GUI::Elements::Element>
        T* getParent() { return static_cast<T*>(m_parent); }

        template<typename T = GUI::Elements::Element>
        const T* getParent() const { return static_cast<const T*>(m_parent); }

    protected:
        virtual void onAttach() = 0;
    };
};