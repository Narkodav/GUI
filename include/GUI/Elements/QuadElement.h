#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Elements/Primitives/QuadPrimitive.h"
#include "GUI/Instance.h"
#include "GUI/Rendering/DefaultTextureCache.h"

#include "CommonApi/Utilities/EventSystems/SingleCallbackEventSystem.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
	class QuadElement;

	enum class QuadElementEvent
	{
		Pressed,
		Released,
		MovedIn,
		MovedOut,
		Count
	};

	struct QuadElementEventPolicy {
		using Type = QuadElementEvent;

		template<Type T>
		struct Trait {};

		template <Type T>
		static void handleError(std::exception_ptr) {
			std::cerr << "[GUI] Exception in button event callback: " << Trait<T>::s_name << std::endl;
		}
	};
}

template<>
struct GUI::QuadElementEventPolicy::Trait<GUI::QuadElementEvent::Pressed> {
	using Signature = void(const GUI::Pointer& pointer, GUI::QuadElement& button);
	static inline const std::string s_name = "Pressed";
};

template<>
struct GUI::QuadElementEventPolicy::Trait<GUI::QuadElementEvent::Released> {
	using Signature = void(const GUI::Pointer& pointer, GUI::QuadElement& button);
	static inline const std::string s_name = "Released";
};

template<>
struct GUI::QuadElementEventPolicy::Trait<GUI::QuadElementEvent::MovedIn> {
	using Signature = void(const GUI::Pointer& pointer, GUI::QuadElement& button);
	static inline const std::string s_name = "MovedIn";
};

template<>
struct GUI::QuadElementEventPolicy::Trait<GUI::QuadElementEvent::MovedOut> {
	using Signature = void(const GUI::Pointer& pointer, GUI::QuadElement& button);
	static inline const std::string s_name = "MovedOut";
};

namespace GUI
{
	class QuadElement : public Element
	{
	public:
		using EventSystem = Utilities::SingleCallbackEventSystem<QuadElementEventPolicy>;
	private:

		QuadPrimitive m_quad;
		EventSystem m_events;

		bool m_pointerIsInside = false;
		bool m_isPressed = false;

	public:

		QuadPrimitive& getQuad() { return m_quad; }

		template<QuadElementEvent E, typename T>
		auto setCallback(T&& callback) {
			m_events.set<E>(std::forward<T>(callback));
		}

	protected:

		virtual bool onRecord(InstanceInterface& instance) override {
			m_quad.record(instance);
			return true;
		}

        virtual bool onPointerEvent(const PointerEvent& event) override {
			switch(event.type) {
			case PointerEvent::Type::Move:
				onMouseMoved(event.pointer);
				break;
			case PointerEvent::Type::Press:
				onMouseButtonPressed(event.pointer);
				break;
			case PointerEvent::Type::Release:
				onMouseButtonReleased(event.pointer);
				break;
			default:
			}
			return true;
		}

		void onMouseMoved(const Pointer& pointer) {
			bool isInside = m_quad.cursorIsInside(pointer);
			if (isInside == m_pointerIsInside)
				return;
			m_pointerIsInside = isInside;
			if (m_pointerIsInside) m_events.emit<QuadElementEvent::MovedIn>(pointer, *this);
			else m_events.emit<QuadElementEvent::MovedOut>(pointer, *this);
		}

		void onMouseButtonPressed(const Pointer& pointer) {
			if (!m_pointerIsInside || m_isPressed) return;
			m_isPressed = true;
			m_events.emit<QuadElementEvent::Pressed>(pointer, *this);
		}

		void onMouseButtonReleased(const Pointer& pointer) {
			if (!m_isPressed) return;
			m_pointerIsInside = false;
			m_isPressed = false;
			m_events.emit<QuadElementEvent::Released>(pointer, *this);				
		}
	};
}