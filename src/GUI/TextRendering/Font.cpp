#include "Font.h"

namespace GUI
{
    Font::Font(const std::string& path)
    {
        if (FT_New_Face(FTContext::instance(), path.c_str(), 0, &m_face))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load font");

        if (FT_Select_Charmap(m_face, FT_ENCODING_UNICODE)) {
            throw std::runtime_error("ERROR::FREETYPE: Failed to set Unicode charmap");
        }

        m_type = getFontType();
    }

    Font::Type Font::getFontType() const
    {
        if ((m_face->face_flags & FT_FACE_FLAG_COLOR) != 0)
        {
            // Check for COLR/CPAL
            FT_ULong length = 0;
            bool hasColr = !FT_Load_Sfnt_Table(m_face, FT_MAKE_TAG('C', 'O', 'L', 'R'), 0, NULL, &length) && length > 0;
            length = 0;
            bool hasCpal = !FT_Load_Sfnt_Table(m_face, FT_MAKE_TAG('C', 'P', 'A', 'L'), 0, NULL, &length) && length > 0;

            if (hasColr && hasCpal)
                return Type::COLORED_COLR_CPAL;

            // Check for CBDT/CBLC
            length = 0;
            bool hasCbdt = !FT_Load_Sfnt_Table(m_face, FT_MAKE_TAG('C', 'B', 'D', 'T'), 0, NULL, &length) && length > 0;
            length = 0;
            bool hasCblc = !FT_Load_Sfnt_Table(m_face, FT_MAKE_TAG('C', 'B', 'L', 'C'), 0, NULL, &length) && length > 0;

            if (hasCbdt && hasCblc)
                return Type::COLORED_CBDT_CBLC;

            // Check for SBIX
            length = 0;
            if (!FT_Load_Sfnt_Table(m_face, FT_MAKE_TAG('s', 'b', 'i', 'x'), 0, NULL, &length) && length > 0) {
                return Type::COLORED_SBIX;
            }

            // Color font but unknown type
            return Type::COLORED;
        }
        else return Type::NORMAL;
        return Type::COLORED; //couldn't determine colored type
    }

    void Font::setCharSize(int width, int height)
    {
        m_width = width;
        m_height = height;
        if (FT_Set_Pixel_Sizes(m_face, m_width, m_height))
            throw std::runtime_error("ERROR::FREETYPE: Failed to set pixel sizes");
        m_characters.clear();
    }

    void Font::setCharSize(int height) //automatically deduces width
    {
        m_width = 0;
        m_height = height;
        if (FT_Set_Pixel_Sizes(m_face, m_width, m_height))
            throw std::runtime_error("ERROR::FREETYPE: Failed to set pixel sizes");
        m_characters.clear();
    }

    bool Font::glyphHasColor(CharId character) const {
        FT_LayerIterator iterator;
        iterator.p = nullptr;
        FT_UInt layerGlyphIndex;
        FT_UInt layerColorIndex;

        return FT_Get_Color_Glyph_Layer(m_face, character, &layerGlyphIndex, &layerColorIndex, &iterator);
    }

    Font::Char Font::getCharGrayscale(CharId character) const
    {
        Texture charTexture;
        if (FT_Load_Glyph(m_face, character, FT_LOAD_RENDER))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load a character");

        if (!m_face->glyph->bitmap.buffer)
        {
            return Char(character, Texture(),
                glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y));
        }

        charTexture.setFromBitmapFlipped(
            m_face->glyph->bitmap.buffer,
            m_face->glyph->bitmap.pitch,
            m_face->glyph->bitmap.rows,
            Texture::Format::R
        );

        return Char(character, std::move(charTexture),
            glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y));
    }

    Font::Char Font::getCharSibix(CharId character) const
    {

        Texture charTexture;
        if (FT_Load_Glyph(m_face, character, FT_LOAD_COLOR | FT_LOAD_RENDER))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load a character");

        if (!m_face->glyph->bitmap.buffer)
        {
            return Char(character, Texture(),
                glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y));
        }

        charTexture.setFromBitmapFlipped(
            m_face->glyph->bitmap.buffer,
            m_face->glyph->bitmap.pitch,
            m_face->glyph->bitmap.rows,
            Texture::Format::RGBA
        );

        return Char(character, std::move(charTexture),
            glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y), true);
    }

    Font::Char Font::getCharCbdtCblc(CharId character) const
    {

        Texture charTexture;
        if (FT_Load_Glyph(m_face, character, FT_LOAD_COLOR | FT_LOAD_RENDER))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load a character");

        if (!m_face->glyph->bitmap.buffer)
        {
            return Char(character, Texture(),
                glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y));
        }

        charTexture.setFromBitmapFlipped(
            m_face->glyph->bitmap.buffer,
            m_face->glyph->bitmap.pitch,
            m_face->glyph->bitmap.rows,
            Texture::Format::RGBA
        );

        return Char(character, std::move(charTexture),
            glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y), true);
    }

    Font::Char Font::getCharColrCpal(CharId character) const {

        if (!glyphHasColor(character))
            return getCharGrayscale(character);

        // First pass: calculate total bounding box
        //FT_BBox bbox;
        //bbox.xMin = bbox.yMin = 32000;
        //bbox.xMax = bbox.yMax = -32000;

        FT_LayerIterator iterator;
        iterator.p = nullptr;
        FT_UInt layerGlyphIndex;
        FT_UInt layerColorIndex;

        // Get initial metrics
        FT_Load_Glyph(m_face, character, FT_LOAD_DEFAULT);
        int originalLeft = m_face->glyph->bitmap_left;
        int originalTop = m_face->glyph->bitmap_top;

        //// First pass to get total bounds
        //while (FT_Get_Color_Glyph_Layer(m_face, character, &layerGlyphIndex, &layerColorIndex, &iterator)) {
        //    FT_Load_Glyph(m_face, layerGlyphIndex, FT_LOAD_DEFAULT);
        //    FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);

        //    FT_Bitmap* bitmap = &m_face->glyph->bitmap;

        //    // Update bounding box
        //    int x1 = m_face->glyph->bitmap_left;
        //    int y1 = m_face->glyph->bitmap_top - bitmap->rows;
        //    int x2 = x1 + bitmap->width;
        //    int y2 = m_face->glyph->bitmap_top;

        //    bbox.xMin = std::min<FT_Pos>(bbox.xMin, x1);
        //    bbox.yMin = std::min<FT_Pos>(bbox.yMin, y1);
        //    bbox.xMax = std::max<FT_Pos>(bbox.xMax, x2);
        //    bbox.yMax = std::max<FT_Pos>(bbox.yMax, y2);
        //}

        //// Calculate dimensions from bounding box
        //int width = bbox.xMax - bbox.xMin;
        //int height = bbox.yMax - bbox.yMin;

        //// Ensure minimum dimensions
        //width = std::max(1, width);
        //height = std::max(1, height);
         
        int width = m_face->glyph->bitmap.width;
        int height = m_face->glyph->bitmap.rows;

        if (width == 0 || height == 0)
        {
            return Char(character, Texture(),
                glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y));
        }

        // Create bitmap buffer for BGRA
        std::vector<uint8_t> bitmapVector(width * height * 4, 0);

        // Get color layer information
        iterator.p = nullptr;

        // Get palette information
        FT_Color* palette = nullptr;
        FT_Error error = FT_Palette_Select(m_face, 0, &palette);
        if (error || !palette) {
            // Handle error or no palette case
            return Char(character, Texture(),
                glm::ivec2(width, height),
                glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y));
        }

        // Iterate through all layers
        while (FT_Get_Color_Glyph_Layer(m_face, character, &layerGlyphIndex, &layerColorIndex, &iterator)) {
            // Load and render layer
            FT_Load_Glyph(m_face, layerGlyphIndex, FT_LOAD_DEFAULT);
            FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);

            FT_Bitmap* bitmap = &m_face->glyph->bitmap;
            FT_Color color = palette[layerColorIndex];

            // Calculate offsets for proper positioning
            int startX = m_face->glyph->bitmap_left - originalLeft;
            int startY = originalTop - m_face->glyph->bitmap_top;

            // Blend layer
            for (unsigned int y = 0; y < bitmap->rows; y++) {
                for (unsigned int x = 0; x < bitmap->width; x++) {
                    // Calculate destination position
                    int destX = startX + x;
                    int destY = startY + y;

                    // Check bounds
                    if (destX >= 0 && destX < width &&
                        destY >= 0 && destY < height) {

                        unsigned char coverage = bitmap->buffer[y * bitmap->pitch + x];
                        int idx = (destY * width + destX) * 4;

                        // Alpha blending with existing content
                        float src_a = coverage / 255.0f;
                        float dst_a = bitmapVector[idx + 3] / 255.0f;
                        float out_a = src_a + dst_a * (1.0f - src_a);

                        if (out_a > 0.0f) {
                            // Premultiplied alpha blending
                            float src_weight = src_a / out_a;
                            float dst_weight = (dst_a * (1.0f - src_a)) / out_a;

                            bitmapVector[idx + 0] = static_cast<uint8_t>(
                                (color.blue * src_weight +
                                    bitmapVector[idx + 0] * dst_weight));  // B
                            bitmapVector[idx + 1] = static_cast<uint8_t>(
                                (color.green * src_weight +
                                    bitmapVector[idx + 1] * dst_weight));  // G
                            bitmapVector[idx + 2] = static_cast<uint8_t>(
                                (color.red * src_weight +
                                    bitmapVector[idx + 2] * dst_weight));  // R
                            bitmapVector[idx + 3] = static_cast<uint8_t>(out_a * 255.0f); // A
                        }
                    }
                    //else __debugbreak();

                }
            }
        }

        // Step 5: Create texture from bitmap
        Texture charTexture;
        charTexture.setFromBitmapFlipped(
            bitmapVector.data(),
            width,
            height,
            Texture::Format::BGRA
        );
        
        // Step 6: Create and return the Char object
        return Char(character, std::move(charTexture),
            glm::ivec2(width, height),
            glm::ivec2(originalLeft, originalTop),
            glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y), true);
    }

    const Font::Char& Font::getChar(CharId character) const {
        auto it = m_characters.find(character);
        if (it != m_characters.end() && it->second.getHasTexture()) {
            return it->second;
        }
        return m_characters.emplace(character,
            (this->*m_getCharTable[static_cast<size_t>(m_type)])(character)).first->second;
    }

    const Font::Char::Metrics& Font::getCharMetrics(CharId character) const
    {
        auto it = m_characters.find(character);
        if (it != m_characters.end()) {
            return it->second.getMetrics();
        }

        if (FT_Load_Glyph(m_face, character, FT_LOAD_COMPUTE_METRICS))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load a character");

        return m_characters.emplace(character, Char(character,
            glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y))        
        ).first->second.getMetrics();
    }

    hb_script_t Font::detectScript(const std::string& text)
    {
        UErrorCode error = U_ZERO_ERROR;

        icu::UnicodeString utext = icu::UnicodeString::fromUTF8(
            icu::StringPiece(text.data(), text.length())
        );

        if (U_FAILURE(error)) {
            return HB_SCRIPT_COMMON;
        }


        // Get the script of the first character that isn't common/inherited
        for (int32_t i = 0; i < utext.length(); i++) {
            UChar32 c = utext.char32At(i);
            UScriptCode script = uscript_getScript(c, &error);

            if (U_SUCCESS(error) &&
                script != USCRIPT_COMMON &&
                script != USCRIPT_INHERITED) {
                // Convert ICU script code to HarfBuzz script tag
                const char* scriptName = uscript_getName(script);
                return hb_script_from_string(scriptName, -1);
            }

            if (utext.char32At(i) > 0xFFFF) {
                i++; // Skip the low surrogate for surrogate pairs
            }
        }

        return HB_SCRIPT_COMMON;
    }

    hb_direction_t Font::detectDirection(const std::string& text)
    {
        UErrorCode error = U_ZERO_ERROR;
        icu::UnicodeString utext = icu::UnicodeString::fromUTF8(
            icu::StringPiece(text.data(), text.length())
        );

        if (U_FAILURE(error)) {
            return HB_DIRECTION_LTR;
        }

        // Create BiDi object
        UBiDi* bidi = ubidi_open();
        ubidi_setPara(bidi, utext.getBuffer(), utext.length(),
            UBIDI_DEFAULT_LTR, nullptr, &error);

        if (U_SUCCESS(error)) {
            UBiDiDirection dir = ubidi_getBaseDirection(utext.getBuffer(), utext.length());
            ubidi_close(bidi);

            switch (dir) {
            case UBIDI_RTL:
                return HB_DIRECTION_RTL;
            case UBIDI_LTR:
                return HB_DIRECTION_LTR;
            default:
                // Check for vertical scripts
                UScriptCode script = uscript_getScript(utext.char32At(0), &error);                
                if (U_SUCCESS(error)) {
                    if (script == USCRIPT_HAN ||
                        script == USCRIPT_HIRAGANA ||
                        script == USCRIPT_KATAKANA) {
                        return HB_DIRECTION_TTB;
                    }
                }
                return HB_DIRECTION_LTR;
            }
        }

        return HB_DIRECTION_LTR; // fallback
    }

    Font::ShapedText Font::shapeText(const std::string& text) const
    {
        // Create/reset buffer
        hb_buffer_t* buffer = hb_buffer_create();

        //these will have to be deduced automatically somehow
        hb_script_t script = detectScript(text);
        hb_direction_t dir = detectDirection(text);

        hb_buffer_set_direction(buffer, dir);
        hb_buffer_set_script(buffer, script);
        hb_buffer_set_language(buffer, hb_language_from_string("en", -1));

        // Add text to buffer
        hb_buffer_add_utf8(buffer, text.c_str(), -1, 0, -1);
        hb_font_t* hb_font = hb_ft_font_create_referenced(m_face);

        // Shape the text
        hb_shape(hb_font, buffer, nullptr, 0);

        // Get glyph information
        unsigned int glyph_count;
        hb_glyph_info_t* glyph_info = hb_buffer_get_glyph_infos(buffer, &glyph_count);

        // Convert to vector of CharIds
        ShapedText textShaped;
        std::vector<CharId> currentLine;

        for (unsigned int i = 0; i < glyph_count; i++) {
            size_t cluster = glyph_info[i].cluster;
            if (cluster < text.length() && text[cluster] == '\n') {
                if (!currentLine.empty()) {
                    textShaped.push_back(std::move(currentLine));
                    currentLine = std::vector<CharId>();
                }
            }
            else {
                currentLine.push_back(glyph_info[i].codepoint);
            }
        }

        // Don't forget the last line if it exists
        if (!currentLine.empty()) {
            textShaped.push_back(std::move(currentLine));
        }

        hb_buffer_destroy(buffer);
        hb_font_destroy(hb_font);

        return textShaped;
    }

    Font::Text::Metrics Font::getTextMetrics(const Text& text, float scale) const
    {
        Text::Metrics metrics;

        if (text.getShapedText().empty()) {
            metrics.height = 0;
            metrics.width = 0;
            metrics.lineCount = 0;
            metrics.lineHeight = ((m_face->size->metrics.height) >> 6) * scale;
            return metrics;
        }
        
        metrics.lineHeight = ((m_face->size->metrics.height) >> 6) * scale;
        metrics.height = metrics.lineHeight * text.getShapedText().size();
        metrics.lineCount = text.getShapedText().size();
        metrics.width = 0;

        for (size_t i = 0; i < text.getShapedText().size(); i++)
        {
            float lineWidth = 0;
            for (size_t j = 0; j < text.getShapedText()[i].size(); j++)
            {
                const auto& charMetrics = getCharMetrics(text.getShapedText()[i][j]);
                lineWidth += (charMetrics.advance.x >> 6) * scale;
            }
            if (lineWidth > metrics.width)
                metrics.width = lineWidth;
        }

        return metrics;
    }

    static inline void renderChar(const Font::Char& character, const Shader& shader, float x, float y)
    {
        if (character.getTexture().getId() != 0)
        {
            character.getTexture().bind();

            const auto& metrics = character.getMetrics();
            shader.setUniform2f("uPosition", x, y);
            shader.setUniform2f("uSize", metrics.size.x, metrics.size.y);
            shader.setUniform2f("uBearing", metrics.bearing.x, metrics.bearing.y);
            shader.setUniform1i("uIsColor", character.getIsColor());

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }

    void Font::alignedLeftLoop(const GUI::Font::ShapedText& shapedText, const Extent2D& windowCoords, size_t i,
        const Shader& shader, float y, float scale) const
    {
        int x = windowCoords.width;
        for (int j = 0; j < shapedText[i].size(); j++)
        {
            const Char& character = getChar(shapedText[i][j]);

            // render glyph texture over quad
            renderChar(character, shader, x, y);

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (character.getMetrics().advance.x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
    }


    void Font::alignedCenterLoop(const GUI::Font::ShapedText& shapedText, const Extent2D& windowCoords, size_t i,
        const Shader& shader, float y, float scale) const
    {
        float lineWidth = 0;
        for (int j = 0; j < shapedText[i].size(); j++) {
            const Char& character = getChar(shapedText[i][j]);
            lineWidth += (character.getMetrics().advance.x >> 6) * scale;
        }

        float x = windowCoords.width - (lineWidth / 2.0f);

        for (int j = 0; j < shapedText[i].size(); j++)
        {
            const Char& character = getChar(shapedText[i][j]);

            // render glyph texture over quad
            renderChar(character, shader, x, y);

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (character.getMetrics().advance.x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
    }
    
    void Font::alignedRightLoop(const GUI::Font::ShapedText& shapedText, const Extent2D& windowCoords, size_t i,
        const Shader& shader, float y, float scale) const
    {
        const Char& characterLast = getChar(shapedText[i].back());
        int x = windowCoords.width - characterLast.getMetrics().size.x;
        for (int j = shapedText[i].size() - 1; j > 0; j--)
        {
            const Char& character = getChar(shapedText[i][j]);
            const Char& characterPrev = getChar(shapedText[i][j - 1]);

            // render glyph texture over quad
            renderChar(character, shader, x, y);

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x -= (characterPrev.getMetrics().advance.x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        const Char& character = getChar(shapedText[i][0]);

        // render glyph texture over quad
        renderChar(character, shader, x, y);
    }
}