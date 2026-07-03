#include "GUI/Properties/Property.h"
#include "GUI/Elements/Element.h"

namespace GUI
{
    void Property::attach(Element* parent) {
        m_parent = parent;
        if(parent == nullptr) throw std::runtime_error("Cannot attach property to nullptr parent");

        // // Work around Clang's missing implementation of P1787 lookup rules.
        // // Can be simplified to subscribe<ObjectEvent::...>() once implemented.
        // m_sub = m_parent->Object::subscribe<GUI::ObjectEvent::ObjectDestroyed>(&Property::detach, *this);
        onAttach();
    }

    void Property::detach(Object* parent) {
        onDetach(m_parent);
        m_parent = nullptr;
    }
};