#include "Layer.h"

namespace GUI
{
	Layer::Layer(const Window& window, glm::vec2 position /*= glm::vec2(0.0f)*/,
		glm::vec2 size /*= glm::vec2(2.0f)*/) : m_position(position), m_size(size), m_isDirty(true)
	{
		for (int i = 0; i < m_shaders.size(); ++i)
			m_shaders[i].set(shaderPaths[i]);

		m_windowSize = window.getWindowExtent();
		Extent2D sizePixels = fromNDC(size, m_windowSize);
		m_frameBuffer.init(sizePixels.width, sizePixels.height,
			&m_shaders[static_cast<size_t>(Shaders::FLAT_TO_SCREEN_PROJECTION)]);

	}


}