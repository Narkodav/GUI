#pragma once
#include "WindowEvents.h"
#include "IOEvents.h"
#include "MultiThreading/EventSystem.h"
#include "Common.h"

namespace GUI
{
    class GLFWContext
    {       
    private:
        GLFWContext() {
            if (!glfwInit())
                throw std::runtime_error("Failed to initialize GLFW");            
        };

    public:
        GLFWContext(const GLFWContext&) = delete;
        GLFWContext& operator=(const GLFWContext&) = delete;

        GLFWContext(GLFWContext&&) = delete;
        GLFWContext& operator=(GLFWContext&&) = delete;

        ~GLFWContext()
        {
            glfwTerminate();
        }

        static GLFWContext& instance()
        {
            static GLFWContext s;
            return s;
        }
    };

    static GLFWContext& context = GLFWContext::instance();

    class Window
    {
    public:
         
        enum class CursorMode {
            NORMAL = GLFW_CURSOR_NORMAL,
            HIDDEN = GLFW_CURSOR_HIDDEN,
            DISABLED = GLFW_CURSOR_DISABLED  // For raw motion input
        };

        struct Attributes
        {
            enum class Type {
                // Window behavior
                RESIZABLE,                  // bool
                VISIBLE,                    // bool
                DECORATED,                  // bool
                FOCUSED,                    // bool
                AUTO_ICONIFY,               // bool
                FLOATING,                   // bool
                MAXIMIZED,                  // bool
                CENTER_CURSOR,              // bool
                TRANSPARENT_FRAMEBUFFER,    // bool
                FOCUS_ON_SHOW,              // bool

                // OpenGL specific
                OPENGL_DEBUG_CONTEXT,       // bool
                OPENGL_FORWARD_COMPAT,      // bool

                // Framebuffer settings
                SRGB_CAPABLE,               // bool
                DOUBLE_BUFFER,              // bool
                SAMPLES,                    // int

                // Input mode
                CURSOR_MODE,                // CursorMode enum
            };

            // Window behavior
            bool resizable = true;                  // GLFW_RESIZABLE
            bool visible = true;                    // GLFW_VISIBLE
            bool decorated = true;                  // GLFW_DECORATED (title bar, borders)
            bool focused = true;                    // GLFW_FOCUSED
            bool autoIconify = true;                // GLFW_AUTO_ICONIFY
            bool floating = false;                  // GLFW_FLOATING (always on top)
            bool maximized = false;                 // GLFW_MAXIMIZED
            bool centerCursor = true;               // GLFW_CENTER_CURSOR
            bool transparentFramebuffer = false;    // GLFW_TRANSPARENT_FRAMEBUFFER
            bool focusOnShow = true;                // GLFW_FOCUS_ON_SHOW

            CursorMode cursorMode = CursorMode::NORMAL;

            // Framebuffer settings
            int samples = 0;                 // GLFW_SAMPLES (MSAA)
            bool srgbCapable = false;        // GLFW_SRGB_CAPABLE
            bool doubleBuffer = true;        // GLFW_DOUBLEBUFFER

            // OpenGL specific
            bool openglDebugContext = false; // GLFW_OPENGL_DEBUG_CONTEXT
            bool openglForwardCompat = false;// GLFW_OPENGL_FORWARD_COMPAT

        };

    private:
        Attributes m_attributes;

        GLFWwindow* m_window = nullptr;
        Extent2D m_windowExtent; //extent in window coordinates

        std::string m_windowText;
        bool m_initialized = false;

        MT::EventSystem<WindowEventPolicy> m_windowEvents;
        MT::EventSystem<IOEventPolicy> m_ioEvents;

    public:
        Window() : m_window(nullptr),
            m_windowExtent({ 0, 0 }),
            m_windowText(""),
            m_initialized(false) {
        };

        Window(Extent2D windowExtent, const std::string& windowText, const Attributes& attr = Attributes());

        void init(Extent2D windowExtent, const std::string& windowText, const Attributes& attr = Attributes());

        //add these when refactor the event system
        Window(Window&& other) noexcept {
            m_window = std::exchange(other.m_window, nullptr);
            m_windowExtent = std::exchange(other.m_windowExtent, Extent2D{ 0, 0 });
            m_windowText = std::exchange(other.m_windowText, "");
            m_attributes = std::exchange(other.m_attributes, Attributes());

            //this works
            m_windowEvents = std::exchange(other.m_windowEvents, MT::EventSystem<WindowEventPolicy>());
            m_ioEvents = std::exchange(other.m_ioEvents, MT::EventSystem<IOEventPolicy>());

            m_initialized = std::exchange(other.m_initialized, false);
            if (m_window) {
                glfwSetWindowUserPointer(m_window, this);
            }
        };

        Window& operator=(Window&& other) noexcept
        {
            if (this != &other) {
                destroy();
                m_window = std::exchange(other.m_window, nullptr);
                m_windowExtent = std::exchange(other.m_windowExtent, Extent2D{ 0, 0 });
                m_windowText = std::exchange(other.m_windowText, "");
                m_attributes = std::exchange(other.m_attributes, Attributes());

                //this works
                m_windowEvents = std::exchange(other.m_windowEvents, MT::EventSystem<WindowEventPolicy>());
                m_ioEvents = std::exchange(other.m_ioEvents, MT::EventSystem<IOEventPolicy>());

                m_initialized = std::exchange(other.m_initialized, false);
                if (m_window) {
                    glfwSetWindowUserPointer(m_window, this);
                }
            }
            return *this;
        };

        Window(const Window&) noexcept = delete;
        Window& operator=(const Window&) noexcept = delete;

        ~Window() { 
            destroy();
        };

        const Extent2D& getWindowExtent() const { return m_windowExtent; };
        const std::string& getWindowText() const { return m_windowText; };
        GLFWwindow* getWindowHandle() const { return m_window; };
        void destroy();

        bool shouldClose()
        {
            return glfwWindowShouldClose(m_window) == GLFW_TRUE;
        }

        static void pollEvents()
        {
            glfwPollEvents();
        }

        void swapBuffers()
        {
            glfwSwapBuffers(m_window);
        }

        void makeContextCurrent()
        {
            glfwMakeContextCurrent(m_window);
        }

        static void swapInterval(int interval)
        {
            glfwSwapInterval(interval);
        }

        template<WindowEvents E>
        auto registerCallback(std::function<typename WindowEventPolicy::Traits<E>::Signature> callback)
        {
            return m_windowEvents.subscribe<E>(callback);
        }

        template<IOEvents E>
        auto registerCallback(std::function<typename IOEventPolicy::Traits<E>::Signature> callback)
        {
            return m_ioEvents.subscribe<E>(callback);
        }

        private:
        template<Attributes::Type>
        static constexpr bool always_false = false;

        public:
        template<Attributes::Type type, typename T>
        Window& setAttribute(T value) {
            static_assert(always_false<type>, "This attribute can only be set during window creation!");
            return *this;
        };

        template<>
        Window& setAttribute<Attributes::Type::RESIZABLE>(bool value) {
            glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, value);
            m_attributes.resizable = value;
            return *this;
        }

        template<>
        Window& setAttribute<Attributes::Type::VISIBLE>(bool value) {
            glfwSetWindowAttrib(m_window, GLFW_VISIBLE, value);
            m_attributes.visible = value;
            return *this;
        }

        template<>
        Window& setAttribute<Attributes::Type::DECORATED>(bool value) {
            glfwSetWindowAttrib(m_window, GLFW_DECORATED, value);
            m_attributes.decorated = value;
            return *this;
        }

        template<>
        Window& setAttribute<Attributes::Type::FOCUSED>(bool value) {
            glfwSetWindowAttrib(m_window, GLFW_FOCUSED, value);
            m_attributes.focused = value;
            return *this;
        }

        template<>
        Window& setAttribute<Attributes::Type::AUTO_ICONIFY>(bool value) {
            glfwSetWindowAttrib(m_window, GLFW_AUTO_ICONIFY, value);
            m_attributes.autoIconify = value;
            return *this;
        }

        template<>
        Window& setAttribute<Attributes::Type::FLOATING>(bool value) {
            glfwSetWindowAttrib(m_window, GLFW_FLOATING, value);
            m_attributes.floating = value;
            return *this;
        }

        template<>
        Window& setAttribute<Attributes::Type::MAXIMIZED>(bool value) {
            glfwSetWindowAttrib(m_window, GLFW_MAXIMIZED, value);
            m_attributes.maximized = value;
            return *this;
        }

        template<>
        Window& setAttribute<Attributes::Type::CENTER_CURSOR>(bool value) {
            glfwSetWindowAttrib(m_window, GLFW_CENTER_CURSOR, value);
            m_attributes.centerCursor = value;
            return *this;
        }

        template<>
        Window& setAttribute<Attributes::Type::TRANSPARENT_FRAMEBUFFER>(bool value) {
            glfwSetWindowAttrib(m_window, GLFW_TRANSPARENT_FRAMEBUFFER, value);
            m_attributes.transparentFramebuffer = value;
            return *this;
        }

        template<>
        Window& setAttribute<Attributes::Type::FOCUS_ON_SHOW>(bool value) {
            glfwSetWindowAttrib(m_window, GLFW_FOCUS_ON_SHOW, value);
            m_attributes.focusOnShow = value;
            return *this;
        }

        template<>
        Window& setAttribute<Attributes::Type::CURSOR_MODE>(CursorMode value) {
            glfwSetInputMode(m_window, GLFW_CURSOR, static_cast<int>(value));
            m_attributes.cursorMode = value;
            return *this;
        }

#ifdef _DEBUG
        static void APIENTRY static_glDebugOutput(unsigned int source,
            unsigned int type, unsigned int id, unsigned int severity,
            int length, const char* message, const void* userParam);
#endif

    private:
        // Static window callbacks
        static void static_windowResizeCallback     (GLFWwindow* window, int width, int height);
        static void static_windowMoveCallback       (GLFWwindow* window, int x, int y);
        static void static_windowFocusCallback      (GLFWwindow* window, int focused);
        static void static_windowMinimizeCallback   (GLFWwindow* window, int minimized);
        static void static_windowMaximizeCallback   (GLFWwindow* window, int maximized);
        static void static_windowCloseCallback      (GLFWwindow* window);
        static void static_windowRefreshCallback    (GLFWwindow* window);
        static void static_windowScaleCallback      (GLFWwindow* window, float xscale, float yscale);

        // Static IO callbacks
        static void static_keyCallback              (GLFWwindow* window, int key, int scancode, int action, int mods);
        static void static_charCallback             (GLFWwindow* window, unsigned int codepoint);
        static void static_mouseButtonCallback      (GLFWwindow* window, int button, int action, int mods);
        static void static_cursorPosCallback        (GLFWwindow* window, double x, double y);
        static void static_scrollCallback           (GLFWwindow* window, double xoffset, double yoffset);
        static void static_cursorEnterCallback      (GLFWwindow* window, int entered);
    };

}