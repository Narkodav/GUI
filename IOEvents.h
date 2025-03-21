#pragma once
#include "MultiThreading/EventPolicy.h"

namespace GUI
{
    enum class IOEvents
    {
        KEY_PRESSED,
        KEY_RELEASED,
        KEY_REPEATED,
        CHAR_INPUT,
        MOUSE_BUTTON_PRESSED,
        MOUSE_BUTTON_RELEASED,
        MOUSE_MOVED,
        MOUSE_SCROLLED,
        MOUSE_ENTERED,
        NUM
    };

    struct IOEventPolicy : MT::EventPolicy<GUI::IOEvents, static_cast<size_t>(IOEvents::NUM)> {};
}

template<>
template<>
struct GUI::IOEventPolicy::Traits<GUI::IOEvents::KEY_PRESSED> {
    using Signature = void(int key, int scancode, int mods);
};

template<>
template<>
struct GUI::IOEventPolicy::Traits<GUI::IOEvents::KEY_RELEASED> {
    using Signature = void(int key, int scancode, int mods);
};

template<>
template<>
struct GUI::IOEventPolicy::Traits<GUI::IOEvents::KEY_REPEATED> {
    using Signature = void(int key, int scancode, int mods);
};

template<>
template<>
struct GUI::IOEventPolicy::Traits<GUI::IOEvents::CHAR_INPUT> {
    using Signature = void(unsigned int codepoint);
};

template<>
template<>
struct GUI::IOEventPolicy::Traits<GUI::IOEvents::MOUSE_BUTTON_PRESSED> {
    using Signature = void(int button, int mods);
};

template<>
template<>
struct GUI::IOEventPolicy::Traits<GUI::IOEvents::MOUSE_BUTTON_RELEASED> {
    using Signature = void(int button, int mods);
};

template<>
template<>
struct GUI::IOEventPolicy::Traits<GUI::IOEvents::MOUSE_MOVED> {
    using Signature = void(double x, double y);
};

template<>
template<>
struct GUI::IOEventPolicy::Traits<GUI::IOEvents::MOUSE_SCROLLED> {
    using Signature = void(double xoffset, double yoffset);
};

template<>
template<>
struct GUI::IOEventPolicy::Traits<GUI::IOEvents::MOUSE_ENTERED> {
    using Signature = void(bool entered);
};
