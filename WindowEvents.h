#pragma once
#include "MultiThreading/EventPolicy.h"

namespace GUI
{
    enum class WindowEvents
    {
        WINDOW_RESIZED,
        WINDOW_MOVED,
        WINDOW_FOCUSED,
        WINDOW_MINIMIZED,
        WINDOW_MAXIMIZED,
        WINDOW_CLOSED,
        WINDOW_REFRESH,
        WINDOW_CONTENT_SCALE_CHANGED,
        NUM
    };

    struct WindowEventPolicy : MT::EventPolicy<GUI::WindowEvents, static_cast<size_t>(WindowEvents::NUM)> {};
};

template<>
template<>
struct GUI::WindowEventPolicy::Traits<GUI::WindowEvents::WINDOW_RESIZED> {
    using Signature = void(int width, int height);
};

template<>
template<>
struct GUI::WindowEventPolicy::Traits<GUI::WindowEvents::WINDOW_MOVED> {
    using Signature = void(int x, int y);
};

template<>
template<>
struct GUI::WindowEventPolicy::Traits<GUI::WindowEvents::WINDOW_FOCUSED> {
    using Signature = void(bool focused);
};

template<>
template<>
struct GUI::WindowEventPolicy::Traits<GUI::WindowEvents::WINDOW_MINIMIZED> {
    using Signature = void(bool minimized);
};

template<>
template<>
struct GUI::WindowEventPolicy::Traits<GUI::WindowEvents::WINDOW_MAXIMIZED> {
    using Signature = void(bool maximized);
};

template<>
template<>
struct GUI::WindowEventPolicy::Traits<GUI::WindowEvents::WINDOW_CLOSED> {
    using Signature = void();
};

template<>
template<>
struct GUI::WindowEventPolicy::Traits<GUI::WindowEvents::WINDOW_REFRESH> {
    using Signature = void();
};

template<>
template<>
struct GUI::WindowEventPolicy::Traits<GUI::WindowEvents::WINDOW_CONTENT_SCALE_CHANGED> {
    using Signature = void(float xscale, float yscale);
};