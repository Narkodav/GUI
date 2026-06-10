#pragma once
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftcolor.h>
#include <freetype/tttables.h>
#include <freetype/ftsizes.h>

#include <hb.h>          // Core functionality
#include <hb-ft.h>       // FreeType integration
#include <hb-ot.h>

#include <string>
#include <stdexcept>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Graphics/Graphics.h"

#include "GUI/Rendering/TextureDescriptor.h"

namespace GUI
{
    class InstanceInterface;
    class Instance;

    enum class FontType {
        Normal,             // not colored
        Colored,            // unknown colored type
        ColoredColrCpal,    // loads in layers
        ColoredCbdtCblc,    // loads as is
        ColoredSbix,        // needs png decoding
        Count,
    };

    using CharId = hb_codepoint_t;

    class Glypth {
    public:
        struct Metrics {
            glm::ivec2 size;
            glm::ivec2 bearing;
            glm::ivec2 advance;
        };

    private:
        Metrics m_metrics;

        Graphics::Memory m_textureMemory;
        Graphics::Image m_textureImage;
        Graphics::ImageView m_textureView;

        TextureId m_texture;
        CharId m_char;

    public:
        Glypth() = default;

        void create(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
            const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, Graphics::PhysicalDevice physicalDevice,
            Metrics m_metrics, CharId charId, unsigned char* buffer, uint32_t width, uint32_t height, uint32_t pitch, uint32_t bpp, Graphics::Format format);

        void destroy(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device);

        const Metrics& getMetrics() const { return m_metrics; }
        const TextureId& getTexture() const { return m_texture; }
        const CharId& getCharId() const { return m_char; }
    };
}