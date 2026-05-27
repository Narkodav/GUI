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
        using CharId = hb_codepoint_t;
        using ShapedText = std::vector<std::vector<CharId>>; //each new vector is a new line

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

        using CharMap = std::unordered_map<CharId, Char>;

        class Text // helper struct to cache shaped text
        {
        public:
            enum class Alignment
            {
                NONE,
                LEFT,
                CENTER,
                RIGHT
            };

            enum class AlignmentVertical
            {
                NONE,
                TOP,
                MIDDLE,
                BOTTOM
            };

            //everything is in pixels
            struct Metrics {
                float width = 0;        // Total width (widest line)
                float height = 0;       // Total height of all lines
                float lineHeight = 0;   // Height of a single line
                int lineCount = 0;      // Number of lines
            };

        private:
            ShapedText m_shapedText;
            std::string m_originalText;

        public:
            Text() = default;

            Text(const std::string& text, const Font& font) :
                m_originalText(text), m_shapedText(font.shapeText(text)) {};

            Text(Text&&) noexcept = default;
            Text& operator=(Text&&) = default;

            Text(const Text&) = default;
            Text& operator=(const Text&) = default;

            const ShapedText& getShapedText() const { return m_shapedText; };
            const std::string& getOriginalText() const { return m_originalText; };
        };

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
        int m_width = 0;
        int m_height = 0;
        Type m_type;
        mutable CharMap m_characters;
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
        Font(const std::string& path = defaultFontPath);
        ~Font()
        {
            FT_Done_Face(m_face); //might log error
        }

        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;

        Font(Font&& other)
        {
            m_face = other.m_face;
            m_width = other.m_width;
            m_height = other.m_height;
            m_type = other.m_type;
            m_characters = std::move(other.m_characters);
        }

        Font& operator=(Font&& other)
        {
            if (this == &other)
                return *this;

            FT_Done_Face(m_face);

            m_face = other.m_face;
            m_width = other.m_width;
            m_height = other.m_height;
            m_type = other.m_type;
            m_characters = std::move(other.m_characters);
            return *this;
        }

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

        static void setDefaultFont(const std::string& path)
        {
            defaultFontPath = path;
        }

        static const std::string& getDefaultFont()
        {
            return defaultFontPath;
        }

        void setCharSize(int width, int height);
        void setCharSize(int height);

        const CharMap& getChars() const { return m_characters; };

        // alignment and alignmentVertical show which point the coords point to
        // for example if Alignment is middle and AlignmentVertical is bottom, the coords points to the middle bottom part of the rendered text
        // coordinates are in NDC
        template <Font::Text::Alignment alignment, Font::Text::AlignmentVertical alignmentVertical>
        void drawText(const Text& text, glm::vec2 coords,
            float scale, glm::vec3 color, Extent2D windowSize) const
        {
            // activate corresponding render state	
            const Shader& shader = ShaderCache::getInstance().getShader<ShaderCache::ShaderSpecialisation::TEXT>();
            shader.bind();
            shader.setUniform3f("uColor", color.x, color.y, color.z);
            shader.setUniformMat4f("uProjection", getWindowProjectionFlipped(windowSize));
            shader.setUniform1f("uZValue", 0.0f);
            shader.setUniform1f("uScale", scale);

            glBindVertexArray(StaticBillboard::getInstance().getVAO());

            Extent2D windowCoords = fromNDC(coords, windowSize);

            if constexpr (alignmentVertical == Text::AlignmentVertical::TOP)
            {
                windowCoords.height -= m_height;
                drawTextTop<alignment>(shader, text.getShapedText(), windowCoords, scale);
            }
            else if constexpr (alignmentVertical == Text::AlignmentVertical::MIDDLE)
            {
                windowCoords.height -= m_height / 2;
                drawTextMiddle<alignment>(shader, text.getShapedText(), windowCoords, scale);
            }
            else
            {
                windowCoords.height -= m_height;
                drawTextBottom<alignment>(shader, text.getShapedText(), windowCoords, scale);
            }

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }


        // alignment and alignmentVertical show which point the coords point to
        // for example if Alignment is middle and AlignmentVertical is bottom, the coords points to the middle bottom part of the rendered text
        // coordinates are in pixels
        template <Font::Text::Alignment alignment, Font::Text::AlignmentVertical alignmentVertical>
        void drawTextPixelPerfect(const Text& text, glm::ivec2 coords,
            float scale, glm::vec3 color, Extent2D windowSize) const
        {
            // activate corresponding render state	
            const Shader& shader = ShaderCache::getInstance().getShader<ShaderCache::ShaderSpecialisation::TEXT>();
            shader.bind();
            shader.setUniform3f("uColor", color.x, color.y, color.z);
            shader.setUniformMat4f("uProjection", getWindowProjectionFlipped(windowSize));
            shader.setUniform1f("uZValue", 0.0f);
            shader.setUniform1f("uScale", scale);

            glBindVertexArray(StaticBillboard::getInstance().getVAO()); 

            if constexpr (alignmentVertical == Text::AlignmentVertical::TOP)
            {
                Extent2D windowCoords(coords.x, windowSize.height - coords.y - m_height);
                drawTextTop<alignment>(shader, text.getShapedText(), windowCoords, scale);
            }
            else if constexpr (alignmentVertical == Text::AlignmentVertical::MIDDLE)
            {
                Extent2D windowCoords(coords.x, windowSize.height - coords.y - m_height / 2);
                drawTextMiddle<alignment>(shader, text.getShapedText(), windowCoords, scale);
            }
            else
            {
                Extent2D windowCoords(coords.x, windowSize.height - coords.y - m_height);
                drawTextBottom<alignment>(shader, text.getShapedText(), windowCoords, scale);
            }

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        Text::Metrics getTextMetrics(const Text& text, float scale) const;
        Char::Metrics getCharMetrics(CharId character, float scale) const;

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

        template <Font::Text::Alignment alignment>
        void drawTextBottom(const Shader& shader, const GUI::Font::ShapedText& shapedText,
            const Extent2D& windowCoords, float scale) const
        {
            int yBottom = windowCoords.height;

            float textHeight = ((m_face->size->metrics.height) >> 6) * scale * shapedText.size();
            float y = yBottom + textHeight;
            mainDrawLoop<alignment>(shader, shapedText, windowCoords, scale, y);
        }

        template <Font::Text::Alignment alignment>
        void drawTextMiddle(const Shader& shader, const GUI::Font::ShapedText& shapedText,
            const Extent2D& windowCoords, float scale) const
        {
            int yMiddle = windowCoords.height;
            // activate corresponding render state	

            float textHeight = ((m_face->size->metrics.height) >> 6) * scale * shapedText.size();
            float y = yMiddle + (textHeight / 2.0f);
            mainDrawLoop<alignment>(shader, shapedText, windowCoords, scale, y);
        }

        template <Font::Text::Alignment alignment>
        void drawTextTop(const Shader& shader, const GUI::Font::ShapedText& shapedText,
            const Extent2D& windowCoords, float scale) const
        {
            mainDrawLoop<alignment>(shader, shapedText, windowCoords, scale, windowCoords.height);
        }

        template <Font::Text::Alignment alignment>
        void mainDrawLoop(const Shader& shader, const GUI::Font::ShapedText& shapedText,
            const Extent2D& windowCoords, float scale, float y) const
        {
            // iterate through all characters
            for (int i = 0; i < shapedText.size(); i++)
            {
                if constexpr (alignment == Text::Alignment::LEFT)
                    alignedLeftLoop(shapedText, windowCoords, i, shader, y, scale);
                else if constexpr (alignment == Text::Alignment::CENTER)
                    alignedCenterLoop(shapedText, windowCoords, i, shader, y, scale);
                else alignedRightLoop(shapedText, windowCoords, i, shader, y, scale);

                float lineHeight = (m_face->size->metrics.height) >> 6;
                y -= lineHeight * scale;
            }
        }

        void alignedLeftLoop(const GUI::Font::ShapedText& shapedText, const Extent2D& windowCoords, size_t i,
            const Shader& shader, float y, float scale) const;

        void alignedCenterLoop(const GUI::Font::ShapedText& shapedText, const Extent2D& windowCoords, size_t i,
            const Shader& shader, float y, float scale) const;

        void alignedRightLoop(const GUI::Font::ShapedText& shapedText, const Extent2D& windowCoords, size_t i,
            const Shader& shader, float y, float scale) const;
    };

}
