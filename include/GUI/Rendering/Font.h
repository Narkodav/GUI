#pragma once
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftcolor.h>
#include <freetype/tttables.h>

#include <hb.h>          // Core functionality
#include <hb-ft.h>       // FreeType integration
#include <hb-ot.h>

#include <unicode/unistr.h>
#include <unicode/uscript.h>
#include <unicode/ubidi.h>

#include <string>
#include <stdexcept>

#include "Rendering/Texture.h"
#include "Rendering/Shader.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/StaticBillboard.h"
#include "Shaders/ShaderCache.h"

#include "Common.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GUI/Instance.h"

namespace GUI
{
    class FTContext
    {
    public:
        enum class Shaders
        {
            TEXT,
            NUM
        };

    private:
        static inline FT_Library ft;

        FTContext() {
            if (FT_Init_FreeType(&ft))
                throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");

            int major;
            int minor;
            int patch;

            FT_Library_Version(ft, &major, &minor, &patch);

            std::cout << "FreeType version: " << major << "." << minor << "." << patch << std::endl;
        };

    public:
        FTContext(const FTContext&) = delete;
        FTContext& operator=(const FTContext&) = delete;

        FTContext(FTContext&&) = delete;
        FTContext& operator=(FTContext&&) = delete;

        ~FTContext() noexcept
        {
            FT_Done_FreeType(ft); //might log error
        }

        static FTContext& instance()
        {
            static FTContext c;
            return c;
        }

        operator FT_Library() const
        {
            return ft;
        }
    };

    class Font
    {
    public:
        using GlypthId = hb_codepoint_t;

        class Glypth {
        public:
            struct Metrics
            {
                glm::ivec2 size;
                glm::ivec2 bearing;
                glm::ivec2 advance;
            };

        private:
            Metrics m_metrix;
            glm::vec2 m_uvMin;
            glm::vec2 m_uvMax;
            TextureId m_textureIndex;
            GlypthId m_glypth;
        };

        using GlypthMap = std::map<GlypthId, Glypth>;

        enum class Type
        {
            Normal,             // not colored
            Colored,            // unknown colored type
            ColoredColrCpal,    // loads in layers
            ColoredCbdtCblc,    // loads as is
            ColoredSbix,        // needs png decoding
            Count,
        };

    private:
        FT_Face m_face;
        Type m_type;
        size_t m_size;
        mutable GlypthMap m_glypths;
        static inline std::string defaultFontPath = "Fonts/arial.ttf";

        using FontCharLoaderFunc = Font::Char (Font::*)(CharId) const;
        const std::array<FontCharLoaderFunc, static_cast<size_t>(Type::Count)> m_getCharTable = {
                    &Font::getCharGrayscale,   //Normal                                     // tested
                    &Font::getCharGrayscale,   //Colored unknown type default to greyscale  // tested
                    &Font::getCharColrCpal,    //ColoredColrCpal                            // tested
                    &Font::getCharCbdtCblc,    //ColoredCbdtCblc                            // not tested, should work
                    &Font::getCharSibix        //ColoredSbix                                // not tested, should work
        };

    public:
        Font() = default;
        ~Font() { FT_Done_Face(m_face); }

        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;

        Font(Font&& other) = default;
        Font& operator=(Font&& other) = default;

        void create(const std::string& path = defaultFontPath, size_t size);
        void destroy(const std::string& path = defaultFontPath, size_t size);

        void reset(const std::string& path = defaultFontPath)
        {
            FT_Done_Face(m_face);

            if (FT_New_Face(FTContext::instance(), path.c_str(), 0, &m_face))
                throw std::runtime_error("ERROR::FREETYPE: Failed to load font");

            if (FT_Select_Charmap(m_face, FT_ENCODING_UNICODE)) {
                throw std::runtime_error("ERROR::FREETYPE: Failed to set Unicode charmap");
            }

            m_type = getFontType();
        }

        static void setDefaultFont(const std::string& path) { defaultFontPath = path; }
        static const std::string& getDefaultFont() { return defaultFontPath; }

        const CharMap& getChars() const { return m_characters; };

    private:
        inline const Char::Metrics& getCharMetrics(CharId character) const;

        inline const Char& getChar(CharId character) const;
        Char getCharGrayscale(CharId character) const;
        Char getCharSibix(CharId character) const;
        Char getCharCbdtCblc(CharId character) const;
        Char getCharColrCpal(CharId character) const;

        bool glyphHasColor(CharId character) const;

        ShapedText shapeText(const std::string& text) const;

        static hb_script_t detectScript(const std::string& text);
        static hb_direction_t detectDirection(const std::string& text);

        Type getFontType() const;
    };

}
