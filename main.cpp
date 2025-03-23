#include <vector>
#include <chrono>

#include "Window.h"
#include "Font.h"
#include "FrameRateCalculator.h"

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

	
	GUI::Font font("Fonts/seguiemj.ttf");
	font.setCharSize(60);

	auto sub = window.registerCallback<GUI::WindowEvents::WINDOW_RESIZED>(
		[&font](int newWidth, int newHeight) {
			glViewport(0, 0, newWidth, newHeight);
		}
	);

	glViewport(0, 0, window.getWindowExtent().width, window.getWindowExtent().height);

	window.swapInterval(0);

	FrameRateCalculator frc;
	frc.setFrameTimeBuffer(100);

	float deltaTime = 0.0f;
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();

	while(!window.shouldClose())
	{
		end = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count();
		start = std::chrono::steady_clock::now();
		frc.addFrameTime(deltaTime);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GUI::Window::pollEvents();
		
		auto& resolution = window.getWindowExtent();
		
		font.drawTextAlignedBottomCenter(
			"😁😊😂🤣❤😍😒👌\n"
			"üñîçødé FPS: " + std::to_string(frc.updateFrameRate()) + "\n"
			"Resolution: " + std::to_string(resolution.width) + "x"
			+ std::to_string(resolution.height) + "\n"
			"Hello World!\n",
			glm::vec2(0.0f, -0.8f),  // Top-left corner in NDC
			1.f,
			glm::vec3(0.6f, 0.0f, 0.0f),
			window.getWindowExtent()
		);

		window.swapBuffers();
	}
}