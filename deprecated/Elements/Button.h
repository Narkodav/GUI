#pragma once
#include "Element.h"
#include "TextRendering/Font.h"
#include "Rendering/Billboard.h"

namespace GUI
{
	class Button;

	enum class ButtonEvents
	{
		BUTTON_PRESSED,
		BUTTON_RELEASED,
		//BUTTON_CLICKED,
		//BUTTON_DOUBLE_CLICKED,
		BUTTON_MOVED_IN,
		BUTTON_MOVED_OUT,
		NUM
	};

	struct ButtonEventPolicy : MT::EventPolicy <ButtonEvents, fromEnumCast(ButtonEvents::NUM)> {};
}

template<>
template<>
struct GUI::ButtonEventPolicy::Traits<GUI::ButtonEvents::BUTTON_PRESSED> {
	using Signature = void(int button, int mods, GUI::Button& buttonHandle);
};

template<>
template<>
struct GUI::ButtonEventPolicy::Traits<GUI::ButtonEvents::BUTTON_RELEASED> {
	using Signature = void(int button, int mods, GUI::Button& buttonHandle);
};

template<>
template<>
struct GUI::ButtonEventPolicy::Traits<GUI::ButtonEvents::BUTTON_MOVED_IN> {
	using Signature = void(double x, double y, GUI::Button& buttonHandle);
};

template<>
template<>
struct GUI::ButtonEventPolicy::Traits<GUI::ButtonEvents::BUTTON_MOVED_OUT> {
	using Signature = void(double x, double y, GUI::Button& buttonHandle);
};

namespace GUI
{
	class Button : public Element
	{
	private:
		//can probably store them in an array
		struct {
			MT::EventSystem<IOEventPolicy>::Subscription mouseMoved;
			MT::EventSystem<IOEventPolicy>::Subscription buttonPressed;
			MT::EventSystem<IOEventPolicy>::Subscription buttonReleased;
		} m_subscriptions;

		Font m_font; //set by the user
		Font::Text m_text;
		Texture m_texture;
		 
		bool m_mouseIsInside = false;
		bool m_mouseIsPressed = false;
		bool m_fontAndTextSet = false;
		MT::EventSystem<ButtonEventPolicy> m_events;

	protected:

		//expects that framebuffer is bound and m_position and m_dimensions are in framebuffer space
		virtual void onRender(const Extent2D& frameBufferDimensions) {
			Billboard billboard(m_position, glm::vec2(m_position.x + m_dimensions.width, m_position.y + m_dimensions.height));
			billboard.drawToScreen(glm::ortho(0.0f, (float)frameBufferDimensions.width,
				(float)frameBufferDimensions.height, 0.0f), 0, m_texture.getId());
		};

		virtual void onRegistration(Window& window) {
			m_subscriptions.mouseMoved = window.registerCallback<IOEvents::MOUSE_MOVED>([this](double x, double y) {
				onMouseMoved(x, y);
				});
			m_subscriptions.buttonPressed = window.registerCallback<IOEvents::MOUSE_BUTTON_PRESSED>
				([this](int button, int mods) {
				onMouseButtonPressed(button, mods);
					});
			m_subscriptions.buttonReleased = window.registerCallback<IOEvents::MOUSE_BUTTON_RELEASED>
				([this](int button, int mods) {
				onMouseButtonReleased(button, mods);
					});
		};

		virtual void onMouseMoved(double x, double y)
		{
			bool mouseIsInside = cursorIsInside(glm::ivec2(x, y));
			if (!m_visible || mouseIsInside == m_mouseIsInside)
				return;
			m_mouseIsInside = mouseIsInside;
			if (m_mouseIsInside)
				m_events.emit<ButtonEvents::BUTTON_MOVED_IN>(x, y, *this);
			else m_events.emit<ButtonEvents::BUTTON_MOVED_OUT>(x, y, *this);
		}

		virtual void onMouseButtonPressed(int button, int mods)
		{
			if (!m_visible || m_mouseIsPressed || !m_mouseIsInside)
				return;
			m_mouseIsPressed = true;
			m_events.emit<ButtonEvents::BUTTON_PRESSED>(button, mods, *this);
		}

		virtual void onMouseButtonReleased(int button, int mods)
		{
			if (!m_visible || !m_mouseIsPressed)
				return;
			m_mouseIsPressed = false;
			m_events.emit<ButtonEvents::BUTTON_RELEASED>(button, mods, *this);				
		}

	public:

		virtual Button& setTexture(const std::string& path)
		{
			m_texture.load(path);
			if (m_layerInterface)
				m_layerInterface->markDirty();
			return *this;
		}

		virtual Button& setPosition(size_t x, size_t y)
		{
			m_position.x = x;
			m_position.y = y;
			if (m_layerInterface)
				m_layerInterface->markDirty();
			return *this;
		}

		virtual Button& setDimensions(size_t width, size_t height)
		{
			m_dimensions.width = width;
			m_dimensions.height = height;
			if (m_layerInterface)
				m_layerInterface->markDirty();
			return *this;
		}

		virtual Button& setText(const std::string& text)
		{
			m_text = Font::Text(text, m_font);
			if (m_layerInterface)
				m_layerInterface->markDirty();
			return *this;
		}

		virtual Button& setFont(int width, int height, const std::string& path = Font::getDefaultFont())
		{
			m_font.reset(path);
			m_font.setCharSize(width, height);
			m_text = Font::Text(m_text.getOriginalText(), m_font);
			if (m_layerInterface)
				m_layerInterface->markDirty();
			return *this;
		}

		virtual Button& setFont(int height, const std::string& path = Font::getDefaultFont())
		{
			m_font.reset(path);
			m_font.setCharSize(height);
			m_text = Font::Text(m_text.getOriginalText(), m_font);
			if (m_layerInterface)
				m_layerInterface->markDirty();
			return *this;
		}

		virtual Button& setFontSize(int height)
		{
			m_font.setCharSize(height);
			if (m_layerInterface)
				m_layerInterface->markDirty();
			return *this;
		}

		virtual Button& setFontSize(int width, int height)
		{
			m_font.setCharSize(width, height);
			if (m_layerInterface)
				m_layerInterface->markDirty();
			return *this;
		}

		template<ButtonEvents E>
		auto registerCallback(std::function<typename ButtonEventPolicy::Traits<E>::Signature> callback)
		{
			return m_events.subscribe<E>(callback);
		}
	};
}