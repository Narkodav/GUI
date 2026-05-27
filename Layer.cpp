#include "Layer.h"

namespace GUI
{
	Layer::Layer(Window& window) : m_isDirty(true)
	{
		m_windowSize = window.getWindowExtent();
		m_frameBuffer.init(m_windowSize.width, m_windowSize.height);
		;
		m_windowResizeSubscription = window.registerCallback<WindowEvents::WINDOW_RESIZED>(
			[this](int width, int height) { onWindowResize(width, height); }
		);
	}


}