#pragma once
#include "GUI/Properties/LayoutLinear.h"
#include "GUI/Properties/SizeConstraints.h"
#include "GUI/Elements/Element.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
    using LayoutHorizontal = LayoutLinear<LayoutAxis::Horizontal>;
}