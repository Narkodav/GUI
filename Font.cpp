#include "Font.h"

namespace GUI
{
    Font::Font(const std::string& path)
    {
        FTContext::instance();
        if (FT_New_Face(FTContext::instance(), path.c_str(), 0, &m_face))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load font");

        if (FT_Select_Charmap(m_face, FT_ENCODING_UNICODE)) {
            throw std::runtime_error("ERROR::FREETYPE: Failed to set Unicode charmap");
        }

        m_type = getFontType();

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(static_cast<size_t>(Buffers::NUM), m_buffers.data());

        // Bind VAO first
        glBindVertexArray(m_vao);

        // Setup position buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_buffers[static_cast<size_t>(Buffers::POSITION)]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * positions.size(), positions.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(static_cast<size_t>(Locations::POSITION), 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(static_cast<size_t>(Locations::POSITION));

        // Setup UV buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_buffers[static_cast<size_t>(Buffers::UV)]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(static_cast<size_t>(Locations::UV), 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(static_cast<size_t>(Locations::UV));

        // Setup element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[static_cast<size_t>(Buffers::INDEX)]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

        // Unbind VAO
        glBindVertexArray(0);
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
            bool hasCbdt = !FT_Load_Sfnt_Table(m_face, FT_MAKE_TAG('C', 'B', 'D', 'T'), 0, NULL, &length) && length > 0;
            length = 0;
            bool hasCblc = !FT_Load_Sfnt_Table(m_face, FT_MAKE_TAG('C', 'B', 'L', 'C'), 0, NULL, &length) && length > 0;

            if (hasColr && hasCpal)
                return Type::COLORED_CBDT_CBLC;

            // Check for SBIX
            if (!FT_Load_Sfnt_Table(m_face, FT_MAKE_TAG('s', 'b', 'i', 'x'), 0, NULL, &length) && length > 0) {
                return Type::COLORED_SIBIX;
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

    Font::TextMetrics Font::getTextMetrics(const std::string& text, float scale) const
    {
        auto shapedText = shapeText(text);
        TextMetrics metrics;

        if (shapedText.empty()) {
            metrics.height = 0;
            metrics.width = 0;
            metrics.lineCount = 0;
            metrics.lineHeight = ((m_face->size->metrics.height) >> 6) * scale;
            return metrics;
        }

        
        metrics.lineHeight = ((m_face->size->metrics.height) >> 6) * scale;
        metrics.height = metrics.lineHeight * shapedText.size();
        metrics.lineCount = shapedText.size();
        metrics.width = 0;

        for (size_t i = 0; i < shapedText.size(); i++)
        {
            float lineWidth = 0;
            for (size_t j = 0; j < shapedText[i].size(); j++)
            {
                const auto& charMetrics = getCharMetrics(shapedText[i][j]);
                lineWidth += (charMetrics.advance.x >> 6) * scale;
            }
            if (lineWidth > metrics.width)
                metrics.width = lineWidth;
        }

        return metrics;
    }

    //coordinate shows left top corner
    void Font::drawTextAlignedTopLeft(const std::string& text, glm::vec2 coords,
        float scale, glm::vec3 color, Extent2D windowSize) const
    {
        auto shapedText = shapeText(text);
        if (shapedText.empty())
            return;

        Extent2D windowCoords = fromNDC(coords, windowSize);
        int x = windowCoords.width;
        int y = windowCoords.height;
        // activate corresponding render state	
        const Shader* shader;

        shader = &FTContext::instance().getShader(FTContext::Shaders::TEXT);
        shader->bind();
        shader->setUniform3f("uColor", color.x, color.y, color.z);
        shader->setUniformMat4f("uProjection", getWindowProjection(windowSize));
        shader->setUniform1f("uZValue", 0.0f);
        shader->setUniform1f("uScale", scale);

        glBindVertexArray(m_vao);

        // iterate through all characters
        for (int i = 0; i < shapedText.size(); i++)
        {
            for(int j = 0; j < shapedText[i].size(); j++)
            {
                const Char& character = getChar(shapedText[i][j]);

                // render glyph texture over quad
                if (character.getTexture().getId() != 0)
                {
                    character.getTexture().bind();

                    const auto& metrics = character.getMetrics();
                    shader->setUniform2f("uPosition", x, y);
                    shader->setUniform2f("uSize", metrics.size.x, metrics.size.y);
                    shader->setUniform2f("uBearing", metrics.bearing.x, metrics.bearing.y);
                    shader->setUniform1i("uIsColor", character.getIsColor());

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }

                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (character.getMetrics().advance.x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
            }
            float lineHeight = (m_face->size->metrics.height) >> 6;
            y -= lineHeight * scale;
            x = windowCoords.width;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //coordinate shows right top corner
    void Font::drawTextAlignedTopRight(const std::string& text, glm::vec2 coords,
        float scale, glm::vec3 color, Extent2D windowSize) const
    {
        auto shapedText = shapeText(text);
        if (shapedText.empty())
            return;

        Extent2D windowCoords = fromNDC(coords, windowSize);
        int x = windowCoords.width;
        int y = windowCoords.height;
        // activate corresponding render state	
        const Shader* shader;

        shader = &FTContext::instance().getShader(FTContext::Shaders::TEXT);
        shader->bind();
        shader->setUniform3f("uColor", color.x, color.y, color.z);
        shader->setUniformMat4f("uProjection", getWindowProjection(windowSize));
        shader->setUniform1f("uZValue", 0.0f);
        shader->setUniform1f("uScale", scale);

        glBindVertexArray(m_vao);

        // iterate through all characters
        for (int i = 0; i < shapedText.size(); i++)
        {
            const Char& characterLast = getChar(shapedText[i].back());
            x = windowCoords.width - characterLast.getMetrics().size.x;
            for (int j = shapedText[i].size() - 1; j > 0 ; j--)
            {
                const Char& character = getChar(shapedText[i][j]);
                const Char& characterPrev = getChar(shapedText[i][j - 1]);

                // render glyph texture over quad
                if (character.getTexture().getId() != 0)
                {
                    character.getTexture().bind();

                    const auto& metrics = character.getMetrics();
                    shader->setUniform2f("uPosition", x, y);
                    shader->setUniform2f("uSize", metrics.size.x, metrics.size.y);
                    shader->setUniform2f("uBearing", metrics.bearing.x, metrics.bearing.y);
                    shader->setUniform1i("uIsColor", character.getIsColor());

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }

                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x -= (characterPrev.getMetrics().advance.x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
            }
            const Char& character = getChar(shapedText[i][0]);

            // render glyph texture over quad
            if (character.getTexture().getId() != 0)
            {
                character.getTexture().bind();

                const auto& metrics = character.getMetrics();
                shader->setUniform2f("uPosition", x, y);
                shader->setUniform2f("uSize", metrics.size.x, metrics.size.y);
                shader->setUniform2f("uBearing", metrics.bearing.x, metrics.bearing.y);
                shader->setUniform1i("uIsColor", character.getIsColor());

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }

            float lineHeight = (m_face->size->metrics.height) >> 6;
            y -= lineHeight * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //coordinate shows center of the top line
    void Font::drawTextAlignedTopCenter(const std::string& text, glm::vec2 coords,
        float scale, glm::vec3 color, Extent2D windowSize) const
    {
        auto shapedText = shapeText(text);
        if (shapedText.empty())
            return;

        Extent2D windowCoords = fromNDC(coords, windowSize);
        int xCenter = windowCoords.width;
        int y = windowCoords.height;
        // activate corresponding render state	
        const Shader* shader;

        shader = &FTContext::instance().getShader(FTContext::Shaders::TEXT);
        shader->bind();
        shader->setUniform3f("uColor", color.x, color.y, color.z);
        shader->setUniformMat4f("uProjection", getWindowProjection(windowSize));
        shader->setUniform1f("uZValue", 0.0f);
        shader->setUniform1f("uScale", scale);

        glBindVertexArray(m_vao);

        // iterate through all characters
        for (int i = 0; i < shapedText.size(); i++)
        {
            float lineWidth = 0;
            for (int j = 0; j < shapedText[i].size(); j++) {
                const Char& character = getChar(shapedText[i][j]);
                lineWidth += (character.getMetrics().advance.x >> 6) * scale;
            }

            float x = xCenter - (lineWidth / 2.0f);

            for (int j = 0; j < shapedText[i].size(); j++)
            {
                const Char& character = getChar(shapedText[i][j]);

                // render glyph texture over quad
                if (character.getTexture().getId() != 0)
                {
                    character.getTexture().bind();

                    const auto& metrics = character.getMetrics();
                    shader->setUniform2f("uPosition", x, y);
                    shader->setUniform2f("uSize", metrics.size.x, metrics.size.y);
                    shader->setUniform2f("uBearing", metrics.bearing.x, metrics.bearing.y);
                    shader->setUniform1i("uIsColor", character.getIsColor());

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }

                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (character.getMetrics().advance.x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
            }
            float lineHeight = (m_face->size->metrics.height) >> 6;
            y -= lineHeight * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Font::drawTextAlignedMiddleLeft(const std::string& text, glm::vec2 coords,
        float scale, glm::vec3 color, Extent2D windowSize) const
    {
        auto shapedText = shapeText(text);
        if (shapedText.empty())
            return;

        Extent2D windowCoords = fromNDC(coords, windowSize);
        int x = windowCoords.width;
        int yMiddle = windowCoords.height; 
        // activate corresponding render state	
        const Shader* shader;

        shader = &FTContext::instance().getShader(FTContext::Shaders::TEXT);
        shader->bind();
        shader->setUniform3f("uColor", color.x, color.y, color.z);
        shader->setUniformMat4f("uProjection", getWindowProjection(windowSize));
        shader->setUniform1f("uZValue", 0.0f);
        shader->setUniform1f("uScale", scale);

        glBindVertexArray(m_vao);

        float textHeight = ((m_face->size->metrics.height) >> 6) * scale * shapedText.size();
        float y = yMiddle + (textHeight / 2.0f);

        // iterate through all characters
        for (int i = 0; i < shapedText.size(); i++)
        {
            for (int j = 0; j < shapedText[i].size(); j++)
            {
                const Char& character = getChar(shapedText[i][j]);

                // render glyph texture over quad
                if (character.getTexture().getId() != 0)
                {
                    character.getTexture().bind();

                    const auto& metrics = character.getMetrics();
                    shader->setUniform2f("uPosition", x, y);
                    shader->setUniform2f("uSize", metrics.size.x, metrics.size.y);
                    shader->setUniform2f("uBearing", metrics.bearing.x, metrics.bearing.y);
                    shader->setUniform1i("uIsColor", character.getIsColor());

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }

                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (character.getMetrics().advance.x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
            }
            float lineHeight = (m_face->size->metrics.height) >> 6;
            y -= lineHeight * scale;
            x = windowCoords.width;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //coordinate shows right top letter
    void Font::drawTextAlignedMiddleRight(const std::string& text, glm::vec2 coords,
        float scale, glm::vec3 color, Extent2D windowSize) const
    {
        auto shapedText = shapeText(text);
        if (shapedText.empty())
            return;

        Extent2D windowCoords = fromNDC(coords, windowSize);
        int x = windowCoords.width;
        int yMiddle = windowCoords.height;
        // activate corresponding render state	
        const Shader* shader;

        shader = &FTContext::instance().getShader(FTContext::Shaders::TEXT);
        shader->bind();
        shader->setUniform3f("uColor", color.x, color.y, color.z);
        shader->setUniformMat4f("uProjection", getWindowProjection(windowSize));
        shader->setUniform1f("uZValue", 0.0f);
        shader->setUniform1f("uScale", scale);

        glBindVertexArray(m_vao);

        float textHeight = ((m_face->size->metrics.height) >> 6) * scale * shapedText.size();
        float y = yMiddle + (textHeight / 2.0f);

        // iterate through all characters
        for (int i = 0; i < shapedText.size(); i++)
        {
            const Char& characterLast = getChar(shapedText[i].back());
            x = windowCoords.width - characterLast.getMetrics().size.x;
            for (int j = shapedText[i].size() - 1; j > 0; j--)
            {
                const Char& character = getChar(shapedText[i][j]);
                const Char& characterPrev = getChar(shapedText[i][j - 1]);

                // render glyph texture over quad
                if (character.getTexture().getId() != 0)
                {
                    character.getTexture().bind();

                    const auto& metrics = character.getMetrics();
                    shader->setUniform2f("uPosition", x, y);
                    shader->setUniform2f("uSize", metrics.size.x, metrics.size.y);
                    shader->setUniform2f("uBearing", metrics.bearing.x, metrics.bearing.y);
                    shader->setUniform1i("uIsColor", character.getIsColor());

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }

                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x -= (characterPrev.getMetrics().advance.x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
            }
            const Char& character = getChar(shapedText[i][0]);

            // render glyph texture over quad
            if (character.getTexture().getId() != 0)
            {
                character.getTexture().bind();

                const auto& metrics = character.getMetrics();
                shader->setUniform2f("uPosition", x, y);
                shader->setUniform2f("uSize", metrics.size.x, metrics.size.y);
                shader->setUniform2f("uBearing", metrics.bearing.x, metrics.bearing.y);
                shader->setUniform1i("uIsColor", character.getIsColor());

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }

            float lineHeight = (m_face->size->metrics.height) >> 6;
            y -= lineHeight * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Font::drawTextAlignedMiddleCenter(const std::string& text, glm::vec2 coords,
        float scale, glm::vec3 color, Extent2D windowSize) const
    {
        auto shapedText = shapeText(text);
        if (shapedText.empty())
            return;

        Extent2D windowCoords = fromNDC(coords, windowSize);
        int xCenter = windowCoords.width;
        int yMiddle = windowCoords.height;
        // activate corresponding render state	
        const Shader* shader;

        shader = &FTContext::instance().getShader(FTContext::Shaders::TEXT);
        shader->bind();
        shader->setUniform3f("uColor", color.x, color.y, color.z);
        shader->setUniformMat4f("uProjection", getWindowProjection(windowSize));
        shader->setUniform1f("uZValue", 0.0f);
        shader->setUniform1f("uScale", scale);

        glBindVertexArray(m_vao);

        float textHeight = ((m_face->size->metrics.height) >> 6) * scale * shapedText.size();
        float y = yMiddle + (textHeight / 2.0f);

        // iterate through all characters
        for (int i = 0; i < shapedText.size(); i++)
        {
            float lineWidth = 0;
            for (int j = 0; j < shapedText[i].size(); j++) {
                const Char& character = getChar(shapedText[i][j]);
                lineWidth += (character.getMetrics().advance.x >> 6) * scale;
            }

            float x = xCenter - (lineWidth / 2.0f);

            for (int j = 0; j < shapedText[i].size(); j++)
            {
                const Char& character = getChar(shapedText[i][j]);

                // render glyph texture over quad
                if (character.getTexture().getId() != 0)
                {
                    character.getTexture().bind();

                    const auto& metrics = character.getMetrics();
                    shader->setUniform2f("uPosition", x, y);
                    shader->setUniform2f("uSize", metrics.size.x, metrics.size.y);
                    shader->setUniform2f("uBearing", metrics.bearing.x, metrics.bearing.y);
                    shader->setUniform1i("uIsColor", character.getIsColor());

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }

                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (character.getMetrics().advance.x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
            }
            float lineHeight = (m_face->size->metrics.height) >> 6;
            y -= lineHeight * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Font::drawTextAlignedBottomLeft(const std::string& text, glm::vec2 coords,
        float scale, glm::vec3 color, Extent2D windowSize) const
    {
        auto shapedText = shapeText(text);
        if (shapedText.empty())
            return;

        Extent2D windowCoords = fromNDC(coords, windowSize);
        int x = windowCoords.width;
        int yBottom = windowCoords.height;
        // activate corresponding render state	
        const Shader* shader;

        shader = &FTContext::instance().getShader(FTContext::Shaders::TEXT);
        shader->bind();
        shader->setUniform3f("uColor", color.x, color.y, color.z);
        shader->setUniformMat4f("uProjection", getWindowProjection(windowSize));
        shader->setUniform1f("uZValue", 0.0f);
        shader->setUniform1f("uScale", scale);

        glBindVertexArray(m_vao);

        float textHeight = ((m_face->size->metrics.height) >> 6) * scale * shapedText.size();
        float y = yBottom + textHeight;

        // iterate through all characters
        for (int i = shapedText.size() - 1; i > -1; i--)
        {
            for (int j = 0; j < shapedText[i].size(); j++)
            {
                const Char& character = getChar(shapedText[i][j]);

                // render glyph texture over quad
                if (character.getTexture().getId() != 0)
                {
                    character.getTexture().bind();

                    const auto& metrics = character.getMetrics();
                    shader->setUniform2f("uPosition", x, y);
                    shader->setUniform2f("uSize", metrics.size.x, metrics.size.y);
                    shader->setUniform2f("uBearing", metrics.bearing.x, metrics.bearing.y);
                    shader->setUniform1i("uIsColor", character.getIsColor());

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }

                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (character.getMetrics().advance.x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
            }
            float lineHeight = (m_face->size->metrics.height) >> 6;
            y -= lineHeight * scale;
            x = windowCoords.width;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Font::drawTextAlignedBottomRight(const std::string& text, glm::vec2 coords,
        float scale, glm::vec3 color, Extent2D windowSize) const
    {
        auto shapedText = shapeText(text);
        if (shapedText.empty())
            return;

        Extent2D windowCoords = fromNDC(coords, windowSize);
        int x = windowCoords.width;
        int yBottom = windowCoords.height;
        // activate corresponding render state	
        const Shader* shader;

        shader = &FTContext::instance().getShader(FTContext::Shaders::TEXT);
        shader->bind();
        shader->setUniform3f("uColor", color.x, color.y, color.z);
        shader->setUniformMat4f("uProjection", getWindowProjection(windowSize));
        shader->setUniform1f("uZValue", 0.0f);
        shader->setUniform1f("uScale", scale);

        glBindVertexArray(m_vao);

        float textHeight = ((m_face->size->metrics.height) >> 6) * scale * shapedText.size();
        float y = yBottom + textHeight;

        // iterate through all characters
        for (int i = 0; i < shapedText.size(); i++)
        {
            const Char& characterLast = getChar(shapedText[i].back());
            x = windowCoords.width - characterLast.getMetrics().size.x;
            for (int j = shapedText[i].size() - 1; j > 0; j--)
            {
                const Char& character = getChar(shapedText[i][j]);
                const Char& characterPrev = getChar(shapedText[i][j - 1]);

                // render glyph texture over quad
                if (character.getTexture().getId() != 0)
                {
                    character.getTexture().bind();

                    const auto& metrics = character.getMetrics();
                    shader->setUniform2f("uPosition", x, y);
                    shader->setUniform2f("uSize", metrics.size.x, metrics.size.y);
                    shader->setUniform2f("uBearing", metrics.bearing.x, metrics.bearing.y);
                    shader->setUniform1i("uIsColor", character.getIsColor());

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }

                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x -= (characterPrev.getMetrics().advance.x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
            }
            const Char& character = getChar(shapedText[i][0]);

            // render glyph texture over quad
            if (character.getTexture().getId() != 0)
            {
                character.getTexture().bind();

                const auto& metrics = character.getMetrics();
                shader->setUniform2f("uPosition", x, y);
                shader->setUniform2f("uSize", metrics.size.x, metrics.size.y);
                shader->setUniform2f("uBearing", metrics.bearing.x, metrics.bearing.y);
                shader->setUniform1i("uIsColor", character.getIsColor());

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }

            float lineHeight = (m_face->size->metrics.height) >> 6;
            y -= lineHeight * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Font::drawTextAlignedBottomCenter(const std::string& text, glm::vec2 coords,
        float scale, glm::vec3 color, Extent2D windowSize) const
    {
        auto shapedText = shapeText(text);
        if (shapedText.empty())
            return;

        Extent2D windowCoords = fromNDC(coords, windowSize);
        int xCenter = windowCoords.width;
        int yBottom = windowCoords.height;
        // activate corresponding render state	
        const Shader* shader;

        shader = &FTContext::instance().getShader(FTContext::Shaders::TEXT);
        shader->bind();
        shader->setUniform3f("uColor", color.x, color.y, color.z);
        shader->setUniformMat4f("uProjection", getWindowProjection(windowSize));
        shader->setUniform1f("uZValue", 0.0f);
        shader->setUniform1f("uScale", scale);

        glBindVertexArray(m_vao);

        float textHeight = ((m_face->size->metrics.height) >> 6) * scale * shapedText.size();
        float y = yBottom + textHeight;

        // iterate through all characters
        for (int i = 0; i < shapedText.size(); i++)
        {
            float lineWidth = 0;
            for (int j = 0; j < shapedText[i].size(); j++) {
                const Char& character = getChar(shapedText[i][j]);
                lineWidth += (character.getMetrics().advance.x >> 6) * scale;
            }

            float x = xCenter - (lineWidth / 2.0f);

            for (int j = 0; j < shapedText[i].size(); j++)
            {
                const Char& character = getChar(shapedText[i][j]);

                // render glyph texture over quad
                if (character.getTexture().getId() != 0)
                {
                    character.getTexture().bind();

                    const auto& metrics = character.getMetrics();
                    shader->setUniform2f("uPosition", x, y);
                    shader->setUniform2f("uSize", metrics.size.x, metrics.size.y);
                    shader->setUniform2f("uBearing", metrics.bearing.x, metrics.bearing.y);
                    shader->setUniform1i("uIsColor", character.getIsColor());

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }

                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (character.getMetrics().advance.x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
            }
            float lineHeight = (m_face->size->metrics.height) >> 6;
            y -= lineHeight * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}