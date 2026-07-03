#include "GUI/Instance.h"

namespace GUI {
    void InstanceInterface::addQuad(const Quad& quad) { m_instance->m_quads.push_back(quad); }
    const FTInstance& InstanceInterface::getFTInstance() const { return m_instance->getFTInstance(); };
    const RenderingContext& InstanceInterface::getRenderingContext() const { return m_instance->getRenderingContext(); }
    TextureId InstanceInterface::registerTexture(Graphics::ImageViewRef view, Graphics::ImageLayout layout //= Graphics::ImageLayout::ShaderReadOnlyOptimal
    ) { return m_instance->registerTexture(view, layout); };
}
