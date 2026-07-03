#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Properties/Property.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>
#include <exception>

namespace GUI
{
    static constexpr int s_maxElementSize = std::numeric_limits<int>::max() >> 7; // 24 bits max, to prevent overflow

	struct Constraints {
        glm::ivec2 minSize = glm::ivec2(0, 0);
        glm::ivec2 maxSize = glm::ivec2(s_maxElementSize, s_maxElementSize);
	};

    class InstanceInterface;
}

namespace GUI {
	enum class ElementEvent {
		ChildAdded,
        ChildRemoved,
		PropertyAdded,
        PropertyRemoved,

        Count,
	};

	struct ElementEventPolicy {
		using Type = ElementEvent;

		template<Type T>
		struct Trait {};

		template <Type T>
		static void handleError(std::exception_ptr) {
			std::cerr << "[GUI] Exception in Element event callback: " << Trait<T>::s_name << std::endl;
		}
	};
}

template<>
struct GUI::ElementEventPolicy::Trait<GUI::ElementEvent::ChildAdded> {
	using Signature = void(GUI::Element* child);
	static inline const std::string_view s_name = "ChildAdded";
};

template<>
struct GUI::ElementEventPolicy::Trait<GUI::ElementEvent::ChildRemoved> {
	using Signature = void(GUI::Element* child);
	static inline const std::string_view s_name = "ChildRemoved";
};

template<>
struct GUI::ElementEventPolicy::Trait<GUI::ElementEvent::PropertyAdded> {
	using Signature = void(GUI::Property* property);
	static inline const std::string_view s_name = "PropertyAdded";
};

template<>
struct GUI::ElementEventPolicy::Trait<GUI::ElementEvent::PropertyRemoved> {
	using Signature = void(GUI::Property* property);
	static inline const std::string_view s_name = "PropertyRemoved";
};

namespace GUI
{
    class ElementBase : public Object {
        GUI_DECLARE_OBJECT(ElementBase, Object)
    };

    template<typename T>
    concept ElementConcept = std::derived_from<std::remove_cvref_t<T>, ElementBase>;

    class Element : public ElementBase {
        GUI_DECLARE_OBJECT(Element, ElementBase)
        GUI_DECLARE_OBJECT_EVENT_SYSTEM(Element, ElementBase)

    private:
        Element* m_parent = nullptr;
        std::vector<std::unique_ptr<Element>> m_children;
        std::unordered_map<TypeId, std::unique_ptr<Property>> m_properties;

    public:
        virtual ~Element() {
            for(auto& property : m_properties) {
                property.second->detach();
            }
        }

        template<PropertyConcept T>
        T* addProperty(std::unique_ptr<T>&& property) {
            auto cap = m_properties.emplace(T::propertyFamilyIdStatic(), std::move(property));
            auto* ptr = static_cast<T*>(cap.first->second.get());
            ptr->attach(this);
            m_events.emit<ElementEvent::PropertyAdded>(ptr);
            return ptr;
        }

        template<PropertyConcept T>
        std::unique_ptr<T> removeProperty() {
            auto& it = m_properties.find(T::propertyFamilyIdStatic());
            if (it != m_properties.end()) {
                auto ptr = static_cast<std::unique_ptr<T>>(std::move(*it));
                m_properties.erase(it);
                m_events.emit<ElementEvent::PropertyRemoved>(ptr.get());
                ptr->detach();
                return ptr;
            }
            return nullptr;
        }

        size_t getPropertyCount() const { return m_properties.size(); }

        template<ObjectConcept T>
        T* getProperty() {
            auto it = m_properties.find(T::propertyFamilyIdStatic());
            if(it == m_properties.end()) return nullptr;
            return static_cast<T*>(it->second.get());
        }

        template<ObjectConcept T>
        const T* getProperty() const {
            auto it = m_properties.find(T::propertyFamilyIdStatic());
            if(it == m_properties.end()) return nullptr;
            return static_cast<const T*>(it->second.get());
        }

        template<ElementConcept T>
        T* addChild(std::unique_ptr<T>&& child) {
            child->m_parent = this;
            m_children.push_back(std::move(child));
            auto* ptr = static_cast<T*>(m_children.back().get());
            m_events.emit<ElementEvent::ChildAdded>(ptr);
            return ptr;
        }

        template<ElementConcept T>
        std::unique_ptr<T> removeChild(T* child) {
            auto it = std::find_if(m_children.begin(), m_children.end(),
                [child](const auto& c) { return c.get() == child; });
            if (it != m_children.end()) {
                auto ptr = std::move(*it);
                m_children.erase(it);
                m_events.emit<ElementEvent::ChildRemoved>(ptr.get());
                return ptr;
            }
            return nullptr;
        }

        size_t getChildCount() const { return m_children.size(); }

        template<ObjectConcept T = Element>
        T* getChild(size_t index) { return static_cast<T*>(m_children[index].get()); }

        template<ObjectConcept T = Element>
        const T* getChild(size_t index) const { return static_cast<const T*>(m_children[index].get()); }

        template<ObjectConcept T = Element>
        T* getParent() { return static_cast<T*>(m_parent); }

        template<ObjectConcept T = Element>
        const T* getParent() const { return static_cast<const T*>(m_parent); }

        template<ElementConcept U = Element, ElementConcept T>
        std::unique_ptr<U> swapChild(size_t index, std::unique_ptr<T>&& child) {
            child->m_parent = this;
            auto prior = std::exchange(m_children[index], std::move(child));
            m_events.emit<ElementEvent::ChildRemoved>(prior.get());
            m_events.emit<ElementEvent::ChildAdded>(m_children[index].get());
            return prior;
        }

		Element& setPosition(const glm::ivec2& position) {
            auto prior = getPosition();
            if(prior == position) return *this;
            onSetPosition(position);
            return *this;
		}
		
		Element& setPositionX(int position) {
            auto prior = getPositionX();
            if(prior == position) return *this;
            onSetPositionX(position);
            return *this;
		}

		Element& setPositionY(int position) {
            auto prior = getPositionY();
            if(prior == position) return *this;
            onSetPositionY(position);
            return *this;
		}

		Element& setSize(const glm::ivec2& size) {
            auto prior = getSize();
            if(prior == size) return *this;
            onSetSize(size);
            return *this;
		}

		Element& setSizeX(int size) {
            auto prior = getSizeX();
            if(prior == size) return *this;
            onSetSizeX(size);
            return *this;
		}

		Element& setSizeY(int size) {
            auto prior = getSizeY();
            if(prior == size) return *this;
            onSetSizeY(size);
            return *this;
		}

		virtual glm::ivec2 getPosition() const = 0;
		virtual glm::ivec2 getSize() const = 0;

		virtual int getPositionX() const = 0;
		virtual int getPositionY() const = 0;
		virtual int getSizeX() const = 0;
		virtual int getSizeY() const = 0;

        // These are only valid after a record command
        virtual glm::ivec2 getGlobalPosition() const = 0;
		virtual int getGlobalPositionX() const = 0;
		virtual int getGlobalPositionY() const = 0;

        virtual glm::ivec2 getOffset() const = 0;
		virtual int getOffsetX() const = 0;
		virtual int getOffsetY() const = 0;

        virtual glm::ivec2 getBounds() const = 0;
		virtual int getBoundsX() const = 0;
		virtual int getBoundsY() const = 0;

        // Coord is viewport global
        virtual bool positionIsInside(const glm::ivec2& coord) const = 0;

        const Element& record(GUI::InstanceInterface& instance) const {
            if(!onRecord(instance)) return *this;
            for (const auto& child : m_children) child->record(instance); 
            return *this;
        }

        // Resolves the global state and caches it, after this getOffset and getGlobalPosition are safe to call
        virtual void resolve(glm::ivec2 offset = glm::ivec2(0, 0), 
            glm::ivec2 bounds = glm::ivec2(std::numeric_limits<int>::max(), std::numeric_limits<int>::max())) {
            setOffset(offset).setBounds(bounds);
            for (const auto& child : m_children) {
				child->resolve(offset + getPosition(), getSize()); 
			}
        }

        virtual void debugDump(size_t depth = 0) {
            std::string slash(depth, '-');
            std::cout << slash << "| " << getName() << ":\n";
            std::cout << slash << " Size X: " << getSizeX() << ", Y: " << getSizeY() << "\n";
            std::cout << slash << " Pos X: " << getPositionX() << ", Y: " << getPositionY() << "\n";
            std::cout << slash << " Offset X: " << getOffsetX() << ", Y: " << getOffsetY() << "\n";
            std::cout << slash << " Bounds X: " << getBoundsX() << ", Y: " << getBoundsY() << "\n";
            std::cout << slash << " Global pos X: " << getGlobalPositionX() << ", Y: " << getGlobalPositionY() << "\n";

            for (const auto& child : m_children) {
				child->debugDump(depth + 1); 
			}
        }

    protected:
		virtual Element& setOffset(const glm::ivec2& size) = 0;
		virtual Element& setBounds(const glm::ivec2& size) = 0;

		virtual bool onRecord(InstanceInterface& instance) const = 0;

		virtual void onSetSize(const glm::ivec2& size) = 0;
		virtual void onSetSizeX(int sizeX) = 0;
		virtual void onSetSizeY(int sizeY) = 0;
		virtual void onSetPosition(const glm::ivec2& position) = 0;
		virtual void onSetPositionX(int positionX) = 0;
		virtual void onSetPositionY(int positionY) = 0;
    };
};