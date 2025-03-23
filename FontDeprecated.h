#pragma once
#include <ft2build.h>
#include <freetype/ftcolor.h>
#include <freetype/tttables.h>
#include FT_FREETYPE_H  

#include <hb.h>          // Core functionality
#include <hb-ft.h>       // FreeType integration

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
            TEXT_COLORED,
            NUM
        };

    private:
        static inline FT_Library ft;

        static inline std::array<Shader, static_cast<size_t>(Shaders::NUM)> shaders;
        static inline const std::array<std::string, static_cast<size_t>(Shaders::NUM)> shaderPaths = {
            "Shaders/Text.shader",
            "Shaders/TextColored.shader"        
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

        class Char
        {
        private:
            char m_character;
            Texture m_texture;
            glm::ivec2 m_size;         // Size of glyph
            glm::ivec2 m_bearing;      // Offset from baseline to left/top of glyph
            glm::ivec2 m_advance;      // Offset to advance to next glyph

            Char();
            Char(char character, Texture&& texture, glm::ivec2 size,
                glm::ivec2 bearing, glm::ivec2 advance)
                : m_character(character), m_texture(std::move(texture)), m_size(size),
                m_bearing(bearing), m_advance(advance) {}

        public:
            const Texture& getTexture() const { return m_texture; };
            const char& getChar() const { return m_character; };
            const glm::ivec2& getSize() const { return m_size; };
            const glm::ivec2& getBearing() const { return m_bearing; };
            const glm::ivec2& getAdvance() const { return m_advance; };

            Char(const Char&) = delete;
            Char& operator=(const Char&) = delete;

            Char(Char&&) = default;
            Char& operator=(Char&&) = default;

            friend class Font;
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

        using CharMap = std::unordered_map<char, Char>;

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

        using FontCharLoaderFunc = const Font::Char& (Font::*)(char) const;
        const std::array<FontCharLoaderFunc, static_cast<size_t>(Type::NUM)> m_getCharTable = {
                    & Font::getCharGrayscale,   //NORMAL
                    & Font::getCharGrayscale,   //COLORED unknown type default to greyscale
                    & Font::getCharColrCpal,    //COLORED_COLR_CPAL
                    & Font::getCharCbdtCblc,    //COLORED_CBDT_CBLC
                    & Font::getCharSibix        //COLORED_SIBIX
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
        void drawTextAlignedLeft(std::string text, glm::vec2 coords,
            float scale, glm::vec3 color, Extent2D windowSize) const;

        void drawTextAlignedRight(std::string text, glm::vec2 coords,
            float scale, glm::vec3 color, Extent2D windowSize) const;

    private:
        const Char& getChar(char character) const;
        const Char& getCharGrayscale(char character) const;
        const Char& getCharSibix(char character) const;
        const Char& getCharCbdtCblc(char character) const;
        const Char& getCharColrCpal(char character) const;
        Type getFontType() const;
	};

}
