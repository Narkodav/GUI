#include "GUI/Instance.h"

namespace GUI {
    void InstanceInterface::addQuad(const Quad& quad) { m_instance->m_quads.push_back(quad); }

}
