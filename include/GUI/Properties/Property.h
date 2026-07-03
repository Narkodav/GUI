#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

#include "GUI/Object.h"

namespace GUI
{
    class Element;
}

namespace GUI
{
    class PropertyBase : public Object {
        GUI_DECLARE_OBJECT(PropertyBase, Object)
    };

    template<typename T>
    concept PropertyConcept = std::derived_from<std::remove_cvref_t<T>, PropertyBase>;

#define GUI_DECLARE_PROPERTY_FAMILY(Family)                     \
public:                                                         \
    static TypeId propertyFamilyIdStatic()                      \
    {                                                           \
        return getTypeId<Family>();                             \
    }                                                           \
                                                                \
    virtual TypeId propertyFamilyId() const override final      \
    {                                                           \
        return propertyFamilyIdStatic();                        \
    }

    class Property : public PropertyBase {
        GUI_DECLARE_OBJECT(Property, PropertyBase)
    private:
        GUI::Element* m_parent = nullptr;
        GUI::Object::Subscription m_sub;

    public:
        virtual ~Property() = default;
        void attach(GUI::Element* parent);
        void detach(GUI::Object* parent = nullptr);

        template<ObjectConcept T = GUI::Element>
        T* getParent() { return static_cast<T*>(m_parent); }

        template<ObjectConcept T = GUI::Element>
        const T* getParent() const { return static_cast<const T*>(m_parent); }

        // This is used to index properties inside the elements
        virtual TypeId propertyFamilyId() const = 0;

    protected:
        virtual void onAttach() = 0;
        virtual void onDetach(GUI::Element* parent) = 0;
    };
};