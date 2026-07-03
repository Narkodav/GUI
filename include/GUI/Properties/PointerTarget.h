#pragma once
#include "GUI/Elements/Element.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>
#include <exception>

namespace GUI
{
	class PointerTarget;
}

namespace GUI {
    struct Pointer {
        glm::ivec2 position;        // UI Surface local pixel coordinates
        glm::vec2 scrollDelta;      // ScrollDelta if relevant
        bool pressed;
    };

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
			std::cerr << "[GUI] Exception in PointerTarget event callback: " << Trait<T>::s_name << std::endl;
		}
	};
}

template<>
struct GUI::PointerTargetEventPolicy::Trait<GUI::PointerTargetEvent::Pressed> {
	using Signature = void(const GUI::Pointer& pointer, GUI::PointerTarget& target);
	static inline const std::string_view s_name = "Pressed";
};

template<>
struct GUI::PointerTargetEventPolicy::Trait<GUI::PointerTargetEvent::Released> {
	using Signature = void(const GUI::Pointer& pointer, GUI::PointerTarget& target);
	static inline const std::string_view s_name = "Released";
};

template<>
struct GUI::PointerTargetEventPolicy::Trait<GUI::PointerTargetEvent::MovedIn> {
	using Signature = void(const GUI::Pointer& pointer, GUI::PointerTarget& target);
	static inline const std::string_view s_name = "MovedIn";
};

template<>
struct GUI::PointerTargetEventPolicy::Trait<GUI::PointerTargetEvent::MovedOut> {
	using Signature = void(const GUI::Pointer& pointer, GUI::PointerTarget& target);
	static inline const std::string_view s_name = "MovedOut";
};

namespace GUI
{
	class PointerTarget : public Property {
		GUI_DECLARE_OBJECT(PointerTarget, Property)
		GUI_DECLARE_OBJECT_EVENT_SYSTEM(PointerTarget, Property)
		GUI_DECLARE_PROPERTY_FAMILY(PointerTarget)

	private:
		bool m_pointerIsInside = false;
		bool m_isPressed = false;

	public:
        virtual ~PointerTarget() = default;

		void pointerEvent(const Pointer& pointer) {
			bool isInside = getParent()->positionIsInside(pointer.position);
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
        virtual void onDetach(GUI::Element* parent) override {}

        virtual bool pointerIsInsideImpl(const Pointer& pointer) const {

			const auto& pos = getParent()->getGlobalPosition();
			const auto& size = getParent()->getSize();
            if (pointer.position.x > pos.x && pointer.position.x < pos.x + size.x &&
                pointer.position.y > pos.y && pointer.position.y < pos.y + size.y)
                return true;
            return false;
        }
	};
}