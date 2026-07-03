#pragma once
#include "GUI/Elements/Element.h"
#include "GUI/PropertyDispatch.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
    struct MeasureResult {
        std::vector<Constraints> childConstraints;
        Constraints totalConstraints;
    };

	class Layout : public Property {
		GUI_DECLARE_OBJECT(Layout, Property)
		GUI_DECLARE_PROPERTY_FAMILY(Layout)
	public:
        virtual ~Layout() = default;
		Layout() = default;

		virtual MeasureResult measureFull() = 0;

		// Returns minimal required parent rect for no clipping
		virtual glm::ivec2 measure() = 0;
        // called each frame, sets positions and sizes, returns the additional size required to satisfy sizing constraints
		// If there is free space between elements returned additional size can be negative
        virtual glm::ivec2 arrange() = 0;

	protected:
		virtual void onAttach() override {}
        virtual void onDetach(Element*) override {}
	};
}