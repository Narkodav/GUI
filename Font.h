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
#include "Common.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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

        static inline std::array<Shader, static_cast<size_t>(Shaders::NUM)> shaders;
        static inline const std::array<std::string, static_cast<size_t>(Shaders::NUM)> shaderPaths = {
            "Shaders/Text.shader",
        };

        FTContext() {
            if (FT_Init_FreeType(&ft))
                throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");

            int major;
            int minor;
            int patch;

            FT_Library_Version(ft, &major, &minor, &patch);

            std::cout << "FreeType version: " << major << "." << minor << "." << patch << std::endl;

            for (int i = 0; i < static_cast<size_t>(Shaders::NUM); i++)
                shaders[i].set(shaderPaths[i]);
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
            static FTContext s;
            return s;
        }

        operator FT_Library() const
        {
            return ft;
        }

        static const Shader& getShader(Shaders type)
        {
            return shaders[static_cast<size_t>(type)];
        }
    };

    class Font
    {
    public:
        using CharId = hb_codepoint_t;

        class Char
        {
        public:
            struct Metrics
            {
                glm::ivec2 size;         // Size of glyph
                glm::ivec2 bearing;      // Offset from baseline to left/top of glyph
                glm::ivec2 advance;      // Offset to advance to next glyph
            };

        private:
            bool m_hasTexture = false;
            CharId m_character;
            Texture m_texture;
            bool m_isColor;
            Metrics m_metrics;

            Char();
            Char(CharId character, Texture&& texture, glm::ivec2 size,
                glm::ivec2 bearing, glm::ivec2 advance, bool isColor = false)
                : m_character(character), m_texture(std::move(texture)),
                m_metrics({ size, bearing, advance }),
                m_isColor(isColor), m_hasTexture(true) {
            };

            Char(CharId character, glm::ivec2 size,
                glm::ivec2 bearing, glm::ivec2 advance, bool isColor = false)
                : m_character(character), m_texture(Texture()),
                m_metrics({ size, bearing, advance }),
                m_isColor(isColor), m_hasTexture(false) {
            };

            void addTexture(Texture&& texture)
            {
                m_texture = std::move(texture);
                m_hasTexture = true;
            }

        public:
            
            const Texture& getTexture() const { return m_texture; };
            const CharId& getCharId() const { return m_character; };
            const Metrics& getMetrics() const { return m_metrics; };
            const bool& getIsColor() const { return m_isColor; };
            const bool& getHasTexture() const { return m_hasTexture; };

            Char(const Char&) = delete;
            Char& operator=(const Char&) = delete;

            Char(Char&&) = default;
            Char& operator=(Char&&) = default;

            friend class Font;
        };

        //everything is in pixels
        struct TextMetrics {
            float width;        // Total width (widest line)
            float height;       // Total height of all lines
            float lineHeight;   // Height of a single line
            int lineCount;      // Number of lines
        };

        enum class Buffers
        {
            POSITION,
            UV,
            INDEX,
            NUM,
        };

        enum class Locations
        {
            POSITION = 0,
            UV = 1,
        };

        static inline const std::array<glm::vec2, 4> positions = {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f,  1.0f),
            glm::vec2(0.0f,  1.0f),
        };

        static inline const std::array<glm::vec2, 4> uvs = {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
        };

        static inline const std::array<glm::ivec3, 2> indices = {
            glm::vec3(0, 1, 2),
            glm::vec3(0, 2, 3)
        };

        using CharMap = std::unordered_map<CharId, Char>;
        using ShapedText = std::vector<std::vector<CharId>>; //each new vector is a new line

        enum class Type
        {
            NORMAL,             // not colored
            COLORED,            // unknown colored type
            COLORED_COLR_CPAL,  // loads in layers
            COLORED_CBDT_CBLC,  // loads as is
            COLORED_SIBIX,      // needs png decoding
            NUM,
        };

    private:        
        FT_Face m_face;
        int m_width = 0;
        int m_height = 0;
        Type m_type;
        mutable CharMap m_characters;
        static inline std::string defaultFontPath = "Fonts/arial.ttf";

        GLuint m_vao;
        std::array<GLuint, static_cast<size_t>(Buffers::NUM)> m_buffers;

        using FontCharLoaderFunc = Font::Char (Font::*)(CharId) const;
        const std::array<FontCharLoaderFunc, static_cast<size_t>(Type::NUM)> m_getCharTable = {
                    &Font::getCharGrayscale,   //NORMAL //tested
                    &Font::getCharGrayscale,   //COLORED unknown type default to greyscale // tested
                    &Font::getCharColrCpal,    //COLORED_COLR_CPAL // tested
                    &Font::getCharCbdtCblc,    //COLORED_CBDT_CBLC // not tested, should work
                    &Font::getCharSibix        //COLORED_SIBIX // not tested, should work
        };

    public:
        Font(const std::string& path = defaultFontPath);
        ~Font()
        {
            FT_Done_Face(m_face); //might log error
        }

        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;

        Font(Font&&) = default;
        Font& operator=(Font&&) = default;

        static void setDefaultFont(const std::string& path)
        {
            defaultFontPath = path;
        }

        void setCharSize(int width, int height);
        void setCharSize(int height);

        const CharMap& getChars() const { return m_characters; };

        //positions in NDC

        //Top alignment, y coordinate shows top line
        void drawTextAlignedTopLeft(const std::string& text, glm::vec2 coords,
            float scale, glm::vec3 color, Extent2D windowSize) const;

        void drawTextAlignedTopRight(const std::string& text, glm::vec2 coords,
            float scale, glm::vec3 color, Extent2D windowSize) const;

        void drawTextAlignedTopCenter(const std::string& text, glm::vec2 coords,
            float scale, glm::vec3 color, Extent2D windowSize) const;

        //middle alignment, y coordinate shows the middle of the text
        void drawTextAlignedMiddleLeft(const std::string& text, glm::vec2 coords,
            float scale, glm::vec3 color, Extent2D windowSize) const;

        void drawTextAlignedMiddleRight(const std::string& text, glm::vec2 coords,
            float scale, glm::vec3 color, Extent2D windowSize) const;

        void drawTextAlignedMiddleCenter(const std::string& text, glm::vec2 coords,
            float scale, glm::vec3 color, Extent2D windowSize) const;

        //bottom alignment, y coordinate shows the bottom line
        void drawTextAlignedBottomLeft(const std::string& text, glm::vec2 coords,
            float scale, glm::vec3 color, Extent2D windowSize) const;

        void drawTextAlignedBottomRight(const std::string& text, glm::vec2 coords,
            float scale, glm::vec3 color, Extent2D windowSize) const;

        void drawTextAlignedBottomCenter(const std::string& text, glm::vec2 coords,
            float scale, glm::vec3 color, Extent2D windowSize) const;

        TextMetrics getTextMetrics(const std::string& text, float scale) const;
        Char::Metrics getCharMetrics(CharId, float scale) const;

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
