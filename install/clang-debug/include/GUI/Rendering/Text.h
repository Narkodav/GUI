#pragma once
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftcolor.h>
#include <freetype/tttables.h>
#include <freetype/ftsizes.h>

#include <hb.h>          // Core functionality
#include <hb-ft.h>       // FreeType integration
#include <hb-ot.h>

// #include <unicode/unistr.h>
// #include <unicode/uscript.h>
// #include <unicode/ubidi.h>

#include <string>
#include <stdexcept>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Graphics/Graphics.h"

#include "GUI/Rendering/Font.h"

namespace GUI {

    class Text {
    public:
        struct GlyphData {
            const Glyph* glyph;
            uint32_t utfCode;
        };

        struct LineData {
            size_t offset = 0;
            size_t width = 0;
            size_t widthWithInvisible = 0;
            size_t height = 0;
        };

    private:
        std::vector<GlyphData> m_glyphs;
        std::vector<LineData> m_lineDatas;
        std::string m_bytes;

    public:

        Text() { clearText(); }

        void setText(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
            const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, Graphics::PhysicalDevice physicalDevice,
            const Font::Size& fontSize, std::string_view str) {
            clearText();
            m_bytes = str;
            shapeText(instance, instanceFunctions, functions, device, physicalDevice, fontSize, str);
        }

        void clearText() { 
            m_glyphs.clear();
            m_bytes.clear();
            m_lineDatas.clear();
            m_lineDatas.push_back({ 0, 0, 0 });
        }

        const std::vector<GlyphData>& getGlyphs() const { return m_glyphs; }

        size_t lineCount() const { return m_lineDatas.size() - 1; }

        std::span<const GlyphData> operator[](size_t i) const { return std::span<const GlyphData>(
            m_glyphs.begin() + m_lineDatas[i].offset, m_glyphs.begin() + m_lineDatas[i + 1].offset); }

        const LineData& getLineData(size_t i) const { return m_lineDatas[i]; }

        static bool letterIsInvisible(uint8_t c) {
            return c == '\n' || c == '\r';
        }

    private:
        // static hb_script_t detectScript(std::string_view text) {
        //     UErrorCode error = U_ZERO_ERROR;

        //     icu::UnicodeString utext = icu::UnicodeString::fromUTF8(
        //         icu::StringPiece(text.data(), text.length())
        //     );

        //     if (U_FAILURE(error)) {
        //         return HB_SCRIPT_COMMON;
        //     }


        //     // Get the script of the first character that isn't common/inherited
        //     for (int32_t i = 0; i < utext.length(); i++) {
        //         UChar32 c = utext.char32At(i);
        //         UScriptCode script = uscript_getScript(c, &error);

        //         if (U_SUCCESS(error) &&
        //             script != USCRIPT_COMMON &&
        //             script != USCRIPT_INHERITED) {
        //             // Convert ICU script code to HarfBuzz script tag
        //             const char* scriptName = uscript_getName(script);
        //             return hb_script_from_string(scriptName, -1);
        //         }

        //         if (utext.char32At(i) > 0xFFFF) {
        //             i++; // Skip the low surrogate for surrogate pairs
        //         }
        //     }

        //     return HB_SCRIPT_COMMON;
        // }
        
        // static hb_direction_t detectDirection(std::string_view text) {
        //     UErrorCode error = U_ZERO_ERROR;
        //     icu::UnicodeString utext = icu::UnicodeString::fromUTF8(
        //         icu::StringPiece(text.data(), text.length())
        //     );

        //     if (U_FAILURE(error)) {
        //         return HB_DIRECTION_LTR;
        //     }

        //     // Create BiDi object
        //     UBiDi* bidi = ubidi_open();
        //     ubidi_setPara(bidi, utext.getBuffer(), utext.length(),
        //         UBIDI_DEFAULT_LTR, nullptr, &error);

        //     if (U_SUCCESS(error)) {
        //         UBiDiDirection dir = ubidi_getBaseDirection(utext.getBuffer(), utext.length());
        //         ubidi_close(bidi);

        //         switch (dir) {
        //         case UBIDI_RTL:
        //             return HB_DIRECTION_RTL;
        //         case UBIDI_LTR:
        //             return HB_DIRECTION_LTR;
        //         default:
        //             // Check for vertical scripts
        //             UScriptCode script = uscript_getScript(utext.char32At(0), &error);                
        //             if (U_SUCCESS(error)) {
        //                 if (script == USCRIPT_HAN ||
        //                     script == USCRIPT_HIRAGANA ||
        //                     script == USCRIPT_KATAKANA) {
        //                     return HB_DIRECTION_TTB;
        //                 }
        //             }
        //             return HB_DIRECTION_LTR;
        //         }
        //     }

        //     return HB_DIRECTION_LTR; // fallback
        // }

        static uint32_t decodeUtf8At(std::string_view text, size_t offset)
        {
            if (offset >= text.size())
                return 0;

            uint8_t b0 = text[offset];

            if ((b0 & 0x80) == 0)
                return b0;

            if ((b0 & 0xE0) == 0xC0)
                return ((b0 & 0x1F) << 6) |
                    (text[offset + 1] & 0x3F);

            if ((b0 & 0xF0) == 0xE0)
                return ((b0 & 0x0F) << 12) |
                    ((text[offset + 1] & 0x3F) << 6) |
                    (text[offset + 2] & 0x3F);

            if ((b0 & 0xF8) == 0xF0)
                return ((b0 & 0x07) << 18) |
                    ((text[offset + 1] & 0x3F) << 12) |
                    ((text[offset + 2] & 0x3F) << 6) |
                    (text[offset + 3] & 0x3F);

            return 0;
        }

        void shapeText(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
            const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, Graphics::PhysicalDevice physicalDevice,
            const Font::Size& fontSize, std::string_view text) {
            // Create/reset buffer
            hb_buffer_t* buffer = hb_buffer_create();

            //these will have to be deduced automatically somehow
            // hb_script_t script = detectScript(text);
            // hb_direction_t dir = detectDirection(text);

            hb_script_t script = HB_SCRIPT_COMMON;
            hb_direction_t dir = HB_DIRECTION_LTR;

            hb_buffer_set_direction(buffer, dir);
            hb_buffer_set_script(buffer, script);
            hb_buffer_set_language(buffer, hb_language_from_string("en", -1));

            // Add text to buffer
            hb_buffer_add_utf8(buffer, text.data(), -1, 0, -1);
            hb_font_t* hb_font = hb_ft_font_create_referenced(fontSize.getFace());

            // Shape the text
            hb_shape(hb_font, buffer, nullptr, 0);

            // Get glyph information
            unsigned int glyph_count;
            hb_glyph_info_t* glyph_info = hb_buffer_get_glyph_infos(buffer, &glyph_count);

            // Convert to vector of Glyphs
            // ShapedText textShaped;
            // std::vector<CharId> currentLine;
            
            for (size_t i = 0; i < glyph_count; i++) {
                // size_t cluster = glyph_info[i].cluster;
                // if (cluster < text.length() && text[cluster] == '\n') {
                //     if (!currentLine.empty()) {
                //         textShaped.push_back(std::move(currentLine));
                //         currentLine = std::vector<CharId>();
                //     }
                // }
                // else {
                //     m_glyphs.push_back(glyph_info[i].codepoint);
                // }

                const auto& glyph = fontSize.getGlyph(
                    instance, instanceFunctions, functions, device, 
                    physicalDevice, glyph_info[i].codepoint);
                m_glyphs.push_back({&glyph, decodeUtf8At(text, glyph_info[i].cluster)});

                if(text[glyph_info[i].cluster] == '\n') {
                    m_lineDatas.push_back({ m_glyphs.size(), 0, 0 });
                    m_lineDatas.back().widthWithInvisible += m_glyphs.back().glyph->getMetrics().advance.x >> 6;
                }
                else if(text[glyph_info[i].cluster] == '\r') {
                    m_lineDatas.back().widthWithInvisible += m_glyphs.back().glyph->getMetrics().advance.x >> 6;
                    if(i + 1 < glyph_count && text[glyph_info[i + 1].cluster] == '\n') continue;
                    m_lineDatas.push_back({ m_glyphs.size(), 0, 0 });
                }
                else m_lineDatas.back().width += m_glyphs.back().glyph->getMetrics().advance.x >> 6;
            }
            m_lineDatas.push_back({ m_glyphs.size(), 0, 0 });

            // // Don't forget the last line if it exists
            // if (!currentLine.empty()) {
            //     textShaped.push_back(std::move(currentLine));
            // }

            hb_buffer_destroy(buffer);
            hb_font_destroy(hb_font);
        }
    };
}