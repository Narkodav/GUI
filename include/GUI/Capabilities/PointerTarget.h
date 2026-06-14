#pragma once
#include "CommonApi/Utilities/EventSystems/EventSystem.h"

#include "GUI/Elements/Box.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI::Capabilities
{
	class PointerTarget;
}

namespace GUI {
	enum class PointerTargetEvent {
		Pressed,
		Released,
		MovedIn,
		MovedOut,
		Count
	};

	struct PointerTargetEventPolicy {
		using Type = PointerTargetEvent;

		template<Type T>
		struct Trait {};

		template <Type T>
		static void handleError(std::exception_ptr) {
			std::cerr << "[GUI] Exception in box event callback: " << Trait<T>::s_name << std::endl;
		}
	};
}

template<>
struct GUI::PointerTargetEventPolicy::Trait<GUI::PointerTargetEvent::Pressed> {
	using Signature = void(const GUI::Pointer& pointer, GUI::Capabilities::PointerTarget& target);
	static inline const std::string_view s_name = "Pressed";
};

template<>
struct GUI::PointerTargetEventPolicy::Trait<GUI::PointerTargetEvent::Released> {
	using Signature = void(const GUI::Pointer& pointer, GUI::Capabilities::PointerTarget& target);
	static inline const std::string_view s_name = "Released";
};

template<>
struct GUI::PointerTargetEventPolicy::Trait<GUI::PointerTargetEvent::MovedIn> {
	using Signature = void(const GUI::Pointer& pointer, GUI::Capabilities::PointerTarget& target);
	static inline const std::string_view s_name = "MovedIn";
};

template<>
struct GUI::PointerTargetEventPolicy::Trait<GUI::PointerTargetEvent::MovedOut> {
	using Signature = void(const GUI::Pointer& pointer, GUI::Capabilities::PointerTarget& target);
	static inline const std::string_view s_name = "MovedOut";
};

namespace GUI::Capabilities
{
	class PointerTarget : public Capability {
	public:
		using EventSystem = Utilities::EventSystem<PointerTargetEventPolicy>;
		using Subscription = Utilities::EventSystem<PointerTargetEventPolicy>::Subscription;

	private:
		EventSystem m_events;
		bool m_pointerIsInside = false;
		bool m_isPressed = false;

	public:
        PointerTarget() = default;
        virtual ~PointerTarget() = default;

		template<PointerTargetEvent E, typename T>
		auto subscribe(T&& callback) {
			return m_events.subscribe<E>(std::forward<T>(callback));
		}

		template<PointerTargetEvent E, typename T, typename Handler>
		auto subscribe(T&& callback, Handler& handler) {
			return m_events.subscribe<E>(std::forward<T>(callback), handler);
		}

		template<PointerTargetEvent E>
		PointerTarget& unsubscribe(const Subscription& id) {
			m_events.unsubscribe<E>(id);
			return *this;
		}

		void pointerEvent(const Pointer& pointer) {
			bool isInside = pointerIsInside(pointer);
			if(m_pointerIsInside && !isInside) m_events.emit<PointerTargetEvent::MovedOut>(pointer, *this);
			else if (isInside) m_events.emit<PointerTargetEvent::MovedIn>(pointer, *this);
			m_pointerIsInside = isInside;
			if(m_isPressed && !pointer.pressed) m_events.emit<PointerTargetEvent::Released>(pointer, *this);
			else if (m_pointerIsInside && pointer.pressed) m_events.emit<PointerTargetEvent::Pressed>(pointer, *this);
			m_isPressed = pointer.pressed;
		}

		bool pointerIsInside() const { return m_pointerIsInside; }
		bool pointerIsPressed() const { return m_isPressed; }

	protected:
		virtual void onAttach() override {}

        bool pointerIsInside(const Pointer& pointer) const {
			auto* parent = getParent<Elements::Box>();
			const auto& pos = parent->getPosition();
			const auto& size = parent->getSize();
            if (pointer.position.x > pos.x && pointer.position.x < pos.x + size.x &&
                pointer.position.y > pos.y && pointer.position.y < pos.y + size.y)
                return true;
            return false;
        }
	};
}