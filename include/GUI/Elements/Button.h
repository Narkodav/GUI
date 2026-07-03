#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Instance.h"
#include "GUI/Properties/PointerTarget.h"
#include "GUI/Elements/Label.h"
#include "GUI/PropertyDispatch.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI {

	class Button : public Rectangle {
        GUI_DECLARE_OBJECT(Button, Rectangle)
    private:
        TextureId m_idleTexture     = static_cast<size_t>(DefaultTextureType::DefaultButtonIdleTexture);
        TextureId m_hoveredTexture  = static_cast<size_t>(DefaultTextureType::DefaultButtonHoveredTexture);
        TextureId m_pressedTexture  = static_cast<size_t>(DefaultTextureType::DefaultButtonPressedTexture);
        std::array<GUI::PointerTarget::Subscription, static_cast<size_t>(PointerTargetEvent::Count)> m_subs;

	public:
        Button() {
            setTexture(static_cast<size_t>(DefaultTextureType::DefaultButtonIdleTexture));
            auto* ptr = addProperty(std::make_unique<GUI::PointerTarget>());
            m_subs[static_cast<size_t>(PointerTargetEvent::Pressed)] = ptr->subscribe<PointerTargetEvent::Pressed>(&Button::onPressed, *this);
            m_subs[static_cast<size_t>(PointerTargetEvent::Released)] = ptr->subscribe<PointerTargetEvent::Released>(&Button::onReleased, *this);
            m_subs[static_cast<size_t>(PointerTargetEvent::MovedIn)] = ptr->subscribe<PointerTargetEvent::MovedIn>(&Button::onEntered, *this);
            m_subs[static_cast<size_t>(PointerTargetEvent::MovedOut)] = ptr->subscribe<PointerTargetEvent::MovedOut>(&Button::onLeft, *this);
            addChild(std::make_unique<Label>())->setTexture(static_cast<size_t>(DefaultTextureType::TransparentTexture));
		}

        Button(Utility::PropertyDispatch<GUI::PointerTarget>& dispatch) {
            setTexture(static_cast<size_t>(DefaultTextureType::DefaultButtonIdleTexture));
            auto* ptr = addProperty(std::make_unique<GUI::PointerTarget>());
            m_subs[static_cast<size_t>(PointerTargetEvent::Pressed)] = ptr->subscribe<PointerTargetEvent::Pressed>(&Button::onPressed, *this);
            m_subs[static_cast<size_t>(PointerTargetEvent::Released)] = ptr->subscribe<PointerTargetEvent::Released>(&Button::onReleased, *this);
            m_subs[static_cast<size_t>(PointerTargetEvent::MovedIn)] = ptr->subscribe<PointerTargetEvent::MovedIn>(&Button::onEntered, *this);
            m_subs[static_cast<size_t>(PointerTargetEvent::MovedOut)] = ptr->subscribe<PointerTargetEvent::MovedOut>(&Button::onLeft, *this);
            addChild(std::make_unique<Label>())->setTexture(static_cast<size_t>(DefaultTextureType::TransparentTexture));
            dispatch.registerTarget(ptr);
		}

        virtual ~Button() {
            getPointerTarget()->unsubscribe<PointerTargetEvent::Pressed>(m_subs[static_cast<size_t>(PointerTargetEvent::Pressed)])
                .unsubscribe<PointerTargetEvent::Released>(m_subs[static_cast<size_t>(PointerTargetEvent::Released)])
                .unsubscribe<PointerTargetEvent::MovedIn>(m_subs[static_cast<size_t>(PointerTargetEvent::MovedIn)])
                .unsubscribe<PointerTargetEvent::MovedOut>(m_subs[static_cast<size_t>(PointerTargetEvent::MovedOut)]);
        }

        PointerTarget* getPointerTarget() { return getProperty<PointerTarget>(); }
        const PointerTarget* getPointerTarget() const { return getProperty<PointerTarget>(); }
        Label* getLabel() { return getChild<Label>(0); }
        const Label* getLabel() const { return getChild<Label>(0); }

        Button& setIdleTexture(TextureId id) { m_idleTexture = id; return *this; };
        Button& setHoveredTexture(TextureId id) { m_hoveredTexture = id; return *this; };
        Button& setPressedTexture(TextureId id) { m_pressedTexture = id; return *this; };

	protected:
        virtual void onSetPosition(const glm::ivec2& position) override {
            Rectangle::onSetPosition(position);
        }
        virtual void onSetSize(const glm::ivec2& dimensions) override { 
            Rectangle::onSetSize(dimensions);
            getLabel()->setSize(dimensions);
        }

        virtual void onSetPositionX(int positionX) override { 
            Rectangle::onSetPositionX(positionX);
        }
        virtual void onSetPositionY(int positionY) override { 
            Rectangle::onSetPositionY(positionY);
        }
        virtual void onSetSizeX(int sizeX) override { 
            Rectangle::onSetSizeX(sizeX);
            getLabel()->setSizeX(sizeX);
        }
        virtual void onSetSizeY(int sizeY) override { 
            Rectangle::onSetSizeY(sizeY);
            getLabel()->setSizeY(sizeY);
        }

        void onEntered(const GUI::Pointer& pointer, GUI::PointerTarget& target) {
            if(!target.pointerIsPressed()) setTexture(m_hoveredTexture);
        }
        void onLeft(const GUI::Pointer& pointer, GUI::PointerTarget& target) {
            if(!target.pointerIsPressed()) setTexture(m_idleTexture);
        }
        void onPressed(const GUI::Pointer& pointer, GUI::PointerTarget& target) {
            setTexture(m_pressedTexture);
        }
        void onReleased(const GUI::Pointer& pointer, GUI::PointerTarget& target) {
            if(!target.pointerIsInside()) setTexture(m_idleTexture);
            else setTexture(m_hoveredTexture);
        }
    };
};