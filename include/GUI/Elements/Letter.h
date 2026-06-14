// #pragma once
// #include "Graphics/Graphics.h"

// #include "GUI/Instance.h"

// #include <glm/glm.hpp>
// #include <vector>
// #include <cstdint>

// namespace GUI
// {
// 	class LetterElement : public Element
// 	{
// 	private:
// 		LetterPrimitive m_letter;
// 	public:

//         LetterElement& setPosition(const glm::ivec2& position) { m_letter.setPosition(position); return *this; }
//         LetterElement& setGlypth(const Glypth& glypth) { m_letter.setGlypth(glypth); return *this; }
// 		LetterElement& setGlypth(const Glypth* glypth) { m_letter.setGlypth(glypth); return *this; }

// 		const LetterPrimitive& getLetter() const { return m_letter; }

// 	protected:

// 		virtual bool onRecord(InstanceInterface& instance) const override {
// 			m_letter.getQuad().record(instance);
// 			return true;
// 		}

//         virtual bool onPointerEvent(const PointerEvent&) override {
// 			return true;
// 		}
// 	};
// }