#include "Window.h"

namespace GUI
{
    Window::Window(Extent2D windowExtent, const std::string& windowText, const Attributes& attr)
    {
        init(windowExtent, windowText, attr);
    }

    void Window::init(Extent2D windowExtent, const std::string& windowText, const Attributes& attr)
    {
        m_attributes = attr;
        m_windowExtent = windowExtent;
        m_windowText = windowText;

        //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // might use vulkan in the future
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //window attributes
        glfwWindowHint(GLFW_RESIZABLE, attr.resizable);
        glfwWindowHint(GLFW_VISIBLE, attr.visible);
        glfwWindowHint(GLFW_DECORATED, attr.decorated);
        glfwWindowHint(GLFW_FOCUSED, attr.focused);
        glfwWindowHint(GLFW_AUTO_ICONIFY, attr.autoIconify);
        glfwWindowHint(GLFW_FLOATING, attr.floating);
        glfwWindowHint(GLFW_MAXIMIZED, attr.maximized);
        glfwWindowHint(GLFW_CENTER_CURSOR, attr.centerCursor);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, attr.transparentFramebuffer);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, attr.focusOnShow);
        glfwWindowHint(GLFW_SAMPLES, attr.samples);
        glfwWindowHint(GLFW_SRGB_CAPABLE, attr.srgbCapable);
        glfwWindowHint(GLFW_DOUBLEBUFFER, attr.doubleBuffer);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, attr.openglDebugContext);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, attr.openglForwardCompat);


        m_window = glfwCreateWindow(m_windowExtent.width, m_windowExtent.height,
            m_windowText.c_str(), nullptr, nullptr);

        if (!m_window) {
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwSetWindowUserPointer            (m_window, this);

        // Window events
        glfwSetWindowSizeCallback           (m_window, Window::static_windowResizeCallback);
        glfwSetWindowPosCallback            (m_window, Window::static_windowMoveCallback);
        glfwSetWindowFocusCallback          (m_window, Window::static_windowFocusCallback);
        glfwSetWindowIconifyCallback        (m_window, Window::static_windowMinimizeCallback);
        glfwSetWindowMaximizeCallback       (m_window, Window::static_windowMaximizeCallback);
        glfwSetWindowCloseCallback          (m_window, Window::static_windowCloseCallback);
        glfwSetWindowRefreshCallback        (m_window, Window::static_windowRefreshCallback);
        glfwSetWindowContentScaleCallback   (m_window, Window::static_windowScaleCallback);

        // IO events
        glfwSetKeyCallback                  (m_window, Window::static_keyCallback);
        glfwSetCharCallback                 (m_window, Window::static_charCallback);
        glfwSetMouseButtonCallback          (m_window, Window::static_mouseButtonCallback);
        glfwSetCursorPosCallback            (m_window, Window::static_cursorPosCallback);
        glfwSetScrollCallback               (m_window, Window::static_scrollCallback);
        glfwSetCursorEnterCallback          (m_window, Window::static_cursorEnterCallback);

        glfwSetInputMode                    (m_window, GLFW_CURSOR, static_cast<int>(attr.cursorMode));

        m_initialized = true;
    }

    void Window::destroy()
    {
        if (!m_initialized)
            return;
        glfwDestroyWindow(m_window);
        //this is safe
        m_windowEvents.clear();
        m_ioEvents.clear();
        m_initialized = false;
    }


    // Static window callbacks
    void Window::static_windowResizeCallback(GLFWwindow* window, int width, int height) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_windowExtent.width = width;
        self->m_windowExtent.height = height;
        self->m_windowEvents.emit<WindowEvents::WINDOW_RESIZED>(width, height);
    }

    void Window::static_windowMoveCallback(GLFWwindow* window, int x, int y) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_windowEvents.emit<WindowEvents::WINDOW_MOVED>(x, y);
    }

    void Window::static_windowFocusCallback(GLFWwindow* window, int focused) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_windowEvents.emit<WindowEvents::WINDOW_FOCUSED>(focused == GLFW_TRUE);
    }

    void Window::static_windowMinimizeCallback(GLFWwindow* window, int minimized) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_windowEvents.emit<WindowEvents::WINDOW_MINIMIZED>(minimized == GLFW_TRUE);
    }

    void Window::static_windowMaximizeCallback(GLFWwindow* window, int maximized) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_windowEvents.emit<WindowEvents::WINDOW_MAXIMIZED>(maximized == GLFW_TRUE);
    }

    void Window::static_windowCloseCallback(GLFWwindow* window) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_windowEvents.emit<WindowEvents::WINDOW_CLOSED>();
    }

    void Window::static_windowRefreshCallback(GLFWwindow* window) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_windowEvents.emit<WindowEvents::WINDOW_REFRESH>();
    }

    void Window::static_windowScaleCallback(GLFWwindow* window, float xscale, float yscale) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_windowEvents.emit<WindowEvents::WINDOW_CONTENT_SCALE_CHANGED>(xscale, yscale);
    }

    // Static IO callbacks
    void Window::static_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        switch (action) {
        case GLFW_PRESS:
            self->m_ioEvents.emit<IOEvents::KEY_PRESSED>(key, scancode, mods);
            break;
        case GLFW_RELEASE:
            self->m_ioEvents.emit<IOEvents::KEY_RELEASED>(key, scancode, mods);
            break;
        case GLFW_REPEAT:
            self->m_ioEvents.emit<IOEvents::KEY_REPEATED>(key, scancode, mods);
            break;
        }
    }

    void Window::static_charCallback(GLFWwindow* window, unsigned int codepoint) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_ioEvents.emit<IOEvents::CHAR_INPUT>(codepoint);
    }

    void Window::static_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            self->m_ioEvents.emit<IOEvents::MOUSE_BUTTON_PRESSED>(button, mods);
        }
        else {
            self->m_ioEvents.emit<IOEvents::MOUSE_BUTTON_RELEASED>(button, mods);
        }
    }

    void Window::static_cursorPosCallback(GLFWwindow* window, double x, double y) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_ioEvents.emit<IOEvents::MOUSE_MOVED>(x, y);
    }

    void Window::static_scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_ioEvents.emit<IOEvents::MOUSE_SCROLLED>(xoffset, yoffset);
    }

    void Window::static_cursorEnterCallback(GLFWwindow* window, int entered) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_ioEvents.emit<IOEvents::MOUSE_ENTERED>(entered == GLFW_TRUE);
    }

#ifdef _DEBUG
    void APIENTRY Window::static_glDebugOutput(unsigned int source,
        unsigned int type, unsigned int id, unsigned int severity,
        int length, const char* message, const void* userParam)
    {
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::cerr << "---------------" << std::endl;
        std::cerr << "Debug message (" << id << "): " << message << std::endl;

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:             std::cerr << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cerr << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cerr << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cerr << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cerr << "Source: Other"; break;
        } std::cerr << std::endl;

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:               std::cerr << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Type: Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cerr << "Type: Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cerr << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cerr << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cerr << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cerr << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cerr << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cerr << "Type: Other"; break;
        } std::cerr << std::endl;

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:         std::cerr << "Severity: high"; __debugbreak(); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cerr << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cerr << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "Severity: notification"; break;

        } std::cerr << std::endl;
        std::cerr << std::endl;
    }
#endif // _DEBUG
}