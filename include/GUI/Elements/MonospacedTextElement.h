#pragma once
#include "Graphics/Graphics.h"

#include "GUI/Instance.h"
#include "GUI/Elements/Primitives/MonospacedTextPrimitive.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
    // All the positions and dimensions are represented in Surface pixel coordinates
    class MonospacedTextElement : public Element
    {
    protected:
        MonospacedTextPrimitive m_text;
        
    public:
        MonospacedTextElement() = default;
        ~MonospacedTextElement() = default;
        
        MonospacedTextElement& setPosition(const glm::ivec2& position) { m_text.setPosition(position); return *this; }
        MonospacedTextElement& setSize(const glm::ivec2& size) { m_text.setSize(size); return *this; }
        MonospacedTextElement& setBackgroundTexture(TextureId texture) { m_text.setBackgroundTexture(texture); return *this; }
        MonospacedTextElement& setCellSize(const glm::ivec2& size) { m_text.setCellSize(size); return *this; }
        MonospacedTextElement& setText(const Text& text) { m_text.setText(text); return *this; }
        
	protected:

		virtual bool onRecord(InstanceInterface& instance) override {
			m_text.record(instance);
			return true;
		}

        virtual bool onPointerEvent(const PointerEvent& event) override {
			return true;
		}
    };
};