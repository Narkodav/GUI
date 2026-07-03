#pragma once
#include "CommonApi/Utilities/EventSystems/EventSystem.h"

#include <iostream>
#include <exception>

namespace GUI {
    class Object;

	enum class ObjectEvent {
		ObjectDestroyed,
        Count,
	};

	struct ObjectEventPolicy {
		using Type = ObjectEvent;

		template<Type T>
		struct Trait {};

		template <Type T>
		static void handleError(std::exception_ptr) {
			std::cerr << "[GUI] Exception in Element event callback: " << Trait<T>::s_name << std::endl;
		}
	};
}

template<>
struct GUI::ObjectEventPolicy::Trait<GUI::ObjectEvent::ObjectDestroyed> {
	using Signature = void(GUI::Object* object);
	static inline const std::string_view s_name = "ObjectDestroyed";
};

#define GUI_DECLARE_OBJECT_EVENT_SYSTEM(ClassName, ...)                     \
public:                                                                     \
    using EventSystem = Utilities::EventSystem<ClassName##EventPolicy>;     \
    using Subscription = typename EventSystem::Subscription;                \
                                                                            \
private:                                                                    \
    EventSystem m_events;                                                   \
                                                                            \
public:                                                                     \
    __VA_OPT__(using __VA_ARGS__::subscribe;)                               \
    __VA_OPT__(using __VA_ARGS__::unsubscribe;)                             \
    template<ClassName##Event E, typename T>                                \
    auto subscribe(T&& callback) {                                          \
        return m_events.subscribe<E>(                                       \
            std::forward<T>(callback));                                     \
    }                                                                       \
                                                                            \
    template<ClassName##Event E, typename T, typename Handler>              \
    auto subscribe(T&& callback, Handler& handler) {                        \
        return m_events.subscribe<E>(                                       \
            std::forward<T>(callback), handler);                            \
    }                                                                       \
                                                                            \
    template<ClassName##Event E>                                            \
    ClassName& unsubscribe(const Subscription& id) {                        \
        m_events.template unsubscribe<E>(id);                               \
        return *this;                                                       \
    }

// RTTI Layer, allows usage of framework in non RTTI builds
namespace GUI
{
    using TypeId = size_t;

    inline TypeId nextTypeId() {
        static TypeId next = 0;
        return next++;
    }

    template<typename T>
    TypeId getTypeId() {
        static TypeId id = nextTypeId();
        return id;
    }

    class ObjectBase {};

    template<typename T>
    concept ObjectConcept = std::derived_from<std::remove_cvref_t<T>, ObjectBase>;

    class Object : public ObjectBase {
        GUI_DECLARE_OBJECT_EVENT_SYSTEM(Object)
    public:
        Object() = default;
        virtual ~Object() { m_events.emit<ObjectEvent::ObjectDestroyed>(this); }

        Object(const Object&) = delete;
        Object(Object&&) = delete;
        Object& operator=(const Object&) = delete;
        Object& operator=(Object&&) = delete;

        static TypeId typeIdStatic() { return getTypeId<Object>(); }
        virtual TypeId typeId() const = 0;
        bool isA(TypeId type) const { return type == typeId(); }
        virtual bool isConvertible(TypeId type) const { return type == typeIdStatic(); }

        template<ObjectConcept T>
        T* cast() { return this->isConvertible(T::typeIdStatic()) ? static_cast<T*>(this) : nullptr; }

        template<ObjectConcept T>
        const T* cast() const { return this->isConvertible(T::typeIdStatic()) ? static_cast<const T*>(this) : nullptr; }

        static std::string_view getNameStatic() { return "Object"; }
        virtual std::string_view getName() const { return getNameStatic(); }
    };
};

#define GUI_DECLARE_OBJECT(T, Parent)                                           \
public:                                                                         \
static TypeId typeIdStatic() { return getTypeId<T>(); }                         \
virtual TypeId typeId() const override { return T::typeIdStatic(); }            \
virtual bool isConvertible(TypeId type) const override {                        \
    return type == typeIdStatic() || Parent::isConvertible(type);               \
}                                                                               \
static std::string_view getNameStatic() { return #T; }                          \
virtual std::string_view getName() const override { return getNameStatic(); }