#include <vector>

#include "Window.h"
#include "Font.h"

int main()
{
	GUI::Window window(GUI::Extent2D(800, 600), "Window");

	window.makeContextCurrent();

	if (glewInit() != GLEW_OK)
		std::cout << "glew failed to initiate" << std::endl;
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << version << std::endl;
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glDebugMessageCallback(GUI::Window::static_glDebugOutput, nullptr);
#endif //_DEBUG

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	GUI::Font font;
	font.setCharSize(20); //10 pixels height
	font.renderCharMap();

	window.registerCallback<GUI::WindowEvents::WINDOW_RESIZED>(
		[&font](int newWidth, int newHeight) {
			glViewport(0, 0, newWidth, newHeight);
		}
	);

	glViewport(0, 0, window.getWindowExtent().width, window.getWindowExtent().height);

	window.swapInterval(0);
	while(!window.shouldClose())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GUI::Window::pollEvents();

		font.drawText(
			"FPS: 60\n"
			"Resolution: 800x600\n"
			"Hello World!",
			glm::vec2(0.0f, 0.0f),  // Top-left corner in NDC
			1.f,
			glm::vec4(1.f),
			window.getWindowExtent()
		);

		window.swapBuffers();	
	}
}