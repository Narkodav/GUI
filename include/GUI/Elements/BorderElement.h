#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Instance.h"
#include "GUI/Elements/BorderPrimitive.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
    using BorderElementRegion = BorderPrimitive::Region;
	class BorderElement;

	enum class BorderElementEvent
	{
		Pressed,
		Released,
		MovedIn,
		MovedOut,
		Count
	};

	struct BorderElementEventPolicy {
		using Type = BorderElementEvent;

		template<Type T>
		struct Trait {};

		template <Type T>
		static void handleError(std::exception_ptr) {
			std::cerr << "[GUI] Exception in button event callback: " << Trait<T>::s_name << std::endl;
		}
	};
}

template<>
struct GUI::BorderElementEventPolicy::Trait<GUI::BorderElementEvent::Pressed> {
	using Signature = void(const GUI::Pointer& pointer, GUI::BorderElement& button, BorderElementRegion region);
	static inline const std::string_view s_name = "Pressed";
};

template<>
struct GUI::BorderElementEventPolicy::Trait<GUI::BorderElementEvent::Released> {
	using Signature = void(const GUI::Pointer& pointer, GUI::BorderElement& button, BorderElementRegion region);
	static inline const std::string_view s_name = "Released";
};

template<>
struct GUI::BorderElementEventPolicy::Trait<GUI::BorderElementEvent::MovedIn> {
	using Signature = void(const GUI::Pointer& pointer, GUI::BorderElement& button, BorderElementRegion region);
	static inline const std::string_view s_name = "MovedIn";
};

template<>
struct GUI::BorderElementEventPolicy::Trait<GUI::BorderElementEvent::MovedOut> {
	using Signature = void(const GUI::Pointer& pointer, GUI::BorderElement& button, BorderElementRegion region);
	static inline const std::string_view s_name = "MovedOut";
};

namespace GUI
{
    class BorderElement {
	public:
		using EventSystem = Utilities::SingleCallbackEventSystem<BorderElementEventPolicy>;
	protected:
		EventSystem m_events;
        BorderPrimitive m_borderPrimitive;
        std::array<bool, static_cast<size_t>(BorderElementRegion::Count)> m_pointerIsInside = { false };
        std::array<bool, static_cast<size_t>(BorderElementRegion::Count)> m_isPressed = { false };

    public:
        BorderElement() = default;
        ~BorderElement() = default;

        BorderElement& setPosition(const glm::ivec2& position) { m_borderPrimitive.setPosition(position); return *this; }
        BorderElement& setCenterSize(const glm::ivec2& centerSize) { m_borderPrimitive.setCenterSize(centerSize); return *this; }
        BorderElement& setLeftEdgeWidth(size_t width) { m_borderPrimitive.setLeftEdgeWidth(width); return *this; }
        BorderPrimitive& setTopEdgeWidth(size_t width) { m_borderPrimitive.setTopEdgeWidth(width); return *this; }
        BorderPrimitive& setRightEdgeWidth(size_t width) { m_borderPrimitive.setRightEdgeWidth(width); return *this; }
        BorderPrimitive& setBottomEdgeWidth(size_t width) { m_borderPrimitive.setBottomEdgeWidth(width); return *this; }
        BorderPrimitive& setTexture(Region region, TextureId textureId) { m_borderPrimitive.setTexture(region, textureId); return *this; }
        BorderPrimitive& setParameters(const glm::ivec2& position, const glm::ivec2& centerSize, 
            size_t leftEdgeWidth, size_t rightEdgeWidth, size_t topEdgeWidth, size_t bottomEdgeWidth) { 
                m_borderPrimitive.setParameters(position, centerSize, leftEdgeWidth, 
                    rightEdgeWidth, topEdgeWidth, bottomEdgeWidth ); 
                return *this; 
        }

		template<BorderElementEvent E, typename T>
		auto setCallback(T&& callback) {
			m_events.set<E>(std::forward<T>(callback));
		}

	protected:
		virtual bool onRecord(InstanceInterface& instance) override {
			m_borderPrimitive.record(instance);
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
			default: break;
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
};