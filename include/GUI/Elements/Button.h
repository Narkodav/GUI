#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Instance.h"
#include "GUI/Capabilities/PointerTarget.h"
#include "GUI/Elements/Label.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI::Elements {

	class Button : public Rectangle {
    private:
        TextureId m_idleTexture     = static_cast<size_t>(DefaultTextureType::DefaultButtonIdleTexture);
        TextureId m_hoveredTexture  = static_cast<size_t>(DefaultTextureType::DefaultButtonHoveredTexture);
        TextureId m_pressedTexture  = static_cast<size_t>(DefaultTextureType::DefaultButtonPressedTexture);
        std::array<GUI::Capabilities::PointerTarget::Subscription, static_cast<size_t>(PointerTargetEvent::Count)> m_subs;

	public:
        Button() {
            setTexture(static_cast<size_t>(DefaultTextureType::DefaultButtonIdleTexture));
            auto* ptr = addCapability(std::make_unique<GUI::Capabilities::PointerTarget>());
            m_subs[static_cast<size_t>(PointerTargetEvent::Pressed)] = ptr->subscribe<PointerTargetEvent::Pressed>(&Button::onPressed, *this);
            m_subs[static_cast<size_t>(PointerTargetEvent::Released)] = ptr->subscribe<PointerTargetEvent::Released>(&Button::onReleased, *this);
            m_subs[static_cast<size_t>(PointerTargetEvent::MovedIn)] = ptr->subscribe<PointerTargetEvent::MovedIn>(&Button::onEntered, *this);
            m_subs[static_cast<size_t>(PointerTargetEvent::MovedOut)] = ptr->subscribe<PointerTargetEvent::MovedOut>(&Button::onLeft, *this);

            addChild(std::make_unique<Label>())->setTexture(static_cast<size_t>(DefaultTextureType::TransparentTexture));
		}

        virtual ~Button() {
            getPointerTarget()->unsubscribe<PointerTargetEvent::Pressed>(m_subs[static_cast<size_t>(PointerTargetEvent::Pressed)])
                .unsubscribe<PointerTargetEvent::Released>(m_subs[static_cast<size_t>(PointerTargetEvent::Released)])
                .unsubscribe<PointerTargetEvent::MovedIn>(m_subs[static_cast<size_t>(PointerTargetEvent::MovedIn)])
                .unsubscribe<PointerTargetEvent::MovedOut>(m_subs[static_cast<size_t>(PointerTargetEvent::MovedOut)]);
        }

        Capabilities::PointerTarget* getPointerTarget() { return getCapability<Capabilities::PointerTarget>(0); }
        const Capabilities::PointerTarget* getPointerTarget() const { return getCapability<Capabilities::PointerTarget>(0); }
        Label* getLabel() { return getChild<Label>(0); }
        const Label* getLabel() const { return getChild<Label>(0); }

        Button& setIdleTexture(TextureId id) { m_idleTexture = id; return *this; };
        Button& setHoveredTexture(TextureId id) { m_hoveredTexture = id; return *this; };
        Button& setPressedTexture(TextureId id) { m_pressedTexture = id; return *this; };

        virtual Rectangle& setPosition(const glm::ivec2& position) override {
            Rectangle::setPosition(position);
            getLabel()->setPosition(position); 
            return *this; 
        }
        virtual Rectangle& setSize(const glm::ivec2& dimensions) override { 
            Rectangle::setSize(dimensions);
            getLabel()->setSize(dimensions); 
            return *this; 
        }

        virtual Rectangle& setPositionX(int positionX) override { 
            Rectangle::setPositionX(positionX);
            getLabel()->setPositionX(positionX); 
            return *this; 
        }
        virtual Rectangle& setPositionY(int positionY) override { 
            Rectangle::setPositionY(positionY);
            getLabel()->setPositionY(positionY); 
            return *this; 
        }
        virtual Rectangle& setSizeX(int sizeX) override { 
            Rectangle::setSizeX(sizeX);
            getLabel()->setSizeX(sizeX); 
            return *this; 
        }
        virtual Rectangle& setSizeY(int sizeY) override { 
            Rectangle::setSizeY(sizeY);
            getLabel()->setSizeY(sizeY); 
            return *this; 
        }

	protected:
        void onEntered(const GUI::Pointer& pointer, GUI::Capabilities::PointerTarget& target) {
            if(!target.pointerIsPressed()) setTexture(m_hoveredTexture);
        }
        void onLeft(const GUI::Pointer& pointer, GUI::Capabilities::PointerTarget& target) {
            if(!target.pointerIsPressed()) setTexture(m_idleTexture);
        }
        void onPressed(const GUI::Pointer& pointer, GUI::Capabilities::PointerTarget& target) {
            setTexture(m_pressedTexture);
        }
        void onReleased(const GUI::Pointer& pointer, GUI::Capabilities::PointerTarget& target) {
            if(!target.pointerIsInside()) setTexture(m_idleTexture);
            else setTexture(m_hoveredTexture);
        }
    };
};