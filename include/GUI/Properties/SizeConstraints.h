#pragma once
#include "GUI/Elements/Element.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
	class SizeConstraints : public Property {
		GUI_DECLARE_OBJECT(SizeConstraints, Property)
		GUI_DECLARE_PROPERTY_FAMILY(SizeConstraints)
	private:
		Constraints m_constraints;

	public:
		SizeConstraints() = default;
		SizeConstraints(glm::ivec2 minSize, glm::ivec2 maxSize) {
			m_constraints.minSize = minSize;
			m_constraints.maxSize = maxSize;
		}
		SizeConstraints(Constraints constraints) {
			m_constraints = constraints;
		}
        virtual ~SizeConstraints() = default;

		SizeConstraints& setMinSize(const glm::ivec2& minSize) { m_constraints.minSize = minSize; return *this; }
		SizeConstraints& setMaxSize(const glm::ivec2& maxSize) { m_constraints.maxSize = maxSize; return *this; }
		SizeConstraints& setConstraints(const Constraints& constraints) { m_constraints = constraints; return *this; }

		const glm::ivec2& getMinSize() const { return m_constraints.minSize; }
		const glm::ivec2& getMaxSize() const { return m_constraints.maxSize; }
		const Constraints& getConstraints() const { return m_constraints; }

	protected:
		virtual void onAttach() override {}
        virtual void onDetach(Element* parent) {}
	};
}