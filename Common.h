#pragma once
#pragma execution_character_set("utf-8")

//vendor
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "Utilities/ArrayNd.h"

//c++ std 
#include <iostream>
#include <optional>
#include <array>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <future>

namespace GUI
{
    struct Extent2D
    {
        int width;
        int height;
    };

    // NDC are OpenGl style, 0,0 is the center, y points upward, x points right
    // min is -1 and max is 1
    static inline Extent2D fromNDC(glm::vec2 ndc, Extent2D windowExtent) {
        // NDC is in [-1, 1] range, convert to [0, windowSize] range
        // First convert [-1, 1] to [0, 1] range, then scale to window size
        return Extent2D{
        static_cast<int>((ndc.x + 1.0f) * 0.5f * windowExtent.width),
        static_cast<int>((ndc.y + 1.0f) * 0.5f * windowExtent.height)
        };
    }

    static inline glm::vec2 toNDC(Extent2D extent, Extent2D windowExtent) {
        // Convert from [0, windowSize] to [-1, 1] range
        // First convert to [0, 1] range, then to [-1, 1]
        return glm::vec2{
            (extent.width * 2.0f / windowExtent.width) - 1.0f,
            (extent.height * 2.0f / windowExtent.height) - 1.0f
        };
    }

    // returns a projection matrix that converts pixel coords to NDC
    static inline glm::mat4 getWindowProjection(Extent2D windowExtent)
    {
        return glm::ortho(static_cast<float>(0),
            static_cast<float>(windowExtent.width),
            static_cast<float>(0),
            static_cast<float>(windowExtent.height));
    }

    static inline glm::mat4 getWindowProjectionFlipped(Extent2D windowExtent)
    {
        return glm::ortho(static_cast<float>(0),
            static_cast<float>(windowExtent.width),
            static_cast<float>(windowExtent.height),
            static_cast<float>(0));
    }
}

