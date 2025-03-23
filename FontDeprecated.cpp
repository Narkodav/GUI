#include "FontDeprecated.h"

namespace GUI
{

	Font::Font(const std::string& path)
	{
        FTContext::instance();
		if (FT_New_Face(FTContext::instance(), path.c_str(), 0, &m_face))
			throw std::runtime_error("ERROR::FREETYPE: Failed to load font");

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

    const Font::Char& Font::getCharGrayscale(char character) const
    {
        auto it = m_characters.find(character);
        if (it != m_characters.end()) {
            return it->second;
        }

        Texture charTexture;
        if (FT_Load_Char(m_face, character, FT_LOAD_RENDER))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load a character");

        if (!m_face->glyph->bitmap.buffer)
        {
            return m_characters.emplace(character, Char(character, Texture(),
                glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y))).first->second;
        }

        charTexture.setFromBitmapFlipped(
            m_face->glyph->bitmap.buffer,
            m_face->glyph->bitmap.pitch,
            m_face->glyph->bitmap.rows,
            Texture::Format::R
        );

        return m_characters.emplace(character, Char(character, std::move(charTexture),
            glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y))).first->second;
    }

    const Font::Char& Font::getCharSibix(char character) const
    {
        auto it = m_characters.find(character);
        if (it != m_characters.end()) {
            return it->second;
        }

        Texture charTexture;
        if (FT_Load_Char(m_face, character, FT_LOAD_COLOR | FT_LOAD_RENDER))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load a character");

        if (!m_face->glyph->bitmap.buffer)
        {
            return m_characters.emplace(character, Char(character, Texture(),
                glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y))).first->second;
        }

        charTexture.setFromBitmapFlipped(
            m_face->glyph->bitmap.buffer,
            m_face->glyph->bitmap.pitch,
            m_face->glyph->bitmap.rows,
            Texture::Format::RGBA
        );

        return m_characters.emplace(character, Char(character, std::move(charTexture),
            glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y))).first->second;
    }

    const Font::Char& Font::getCharCbdtCblc(char character) const
    {
        auto it = m_characters.find(character);
        if (it != m_characters.end()) {
            return it->second;
        }

        Texture charTexture;
        if (FT_Load_Char(m_face, character, FT_LOAD_COLOR | FT_LOAD_RENDER))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load a character");

        if (!m_face->glyph->bitmap.buffer)
        {
            return m_characters.emplace(character, Char(character, Texture(),
                glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y))).first->second;
        }

        charTexture.setFromBitmapFlipped(
            m_face->glyph->bitmap.buffer,
            m_face->glyph->bitmap.pitch,
            m_face->glyph->bitmap.rows,
            Texture::Format::RGBA
        );

        return m_characters.emplace(character, Char(character, std::move(charTexture),
            glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y))).first->second;
    }

    const Font::Char& Font::getCharColrCpal(char character) const
    {
        auto it = m_characters.find(character);
        if (it != m_characters.end()) {
            return it->second;
        }
                
        FT_UInt glyphIndex = FT_Get_Char_Index(m_face, character);
        if (FT_Load_Char(m_face, glyphIndex, FT_LOAD_COLOR))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load a character");

        auto width = m_face->glyph->metrics.width >> 6;
        auto height = m_face->glyph->metrics.height >> 6;

        // If glyph has no size, return empty char
        if (width == 0 || height == 0) {
            return m_characters.emplace(character, Char(character, Texture(),
                glm::ivec2(0, 0),
                glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y))).first->second;
        }

        std::vector<unsigned char> bitmapVector;
        FT_LayerIterator iterator;
        FT_UInt layerGlyphIndex;
        FT_UInt colorIndex;

        // Allocate RGBA buffer (4 bytes per pixel)
        bitmapVector.resize(width * height * 4, 0);

        // Get palette data once outside the loop
        FT_Palette_Data palette_data;
        if (FT_Palette_Data_Get(m_face, &palette_data))
            throw std::runtime_error("ERROR::FREETYPE: Failed to get palette data");

        FT_Color* palette;
        if (FT_Palette_Select(m_face, 0, &palette))
            throw std::runtime_error("ERROR::FREETYPE: Failed to select palette");

        // Initialize iterator
        iterator.p = nullptr;

        // Iterate through all layers
        while (FT_Get_Color_Glyph_Layer(m_face, m_face->glyph->glyph_index, &layerGlyphIndex, &colorIndex, &iterator)) {
            // Load this layer's glyph
            if (FT_Load_Glyph(m_face, layerGlyphIndex, FT_LOAD_RENDER)) continue;

            FT_Color color = palette[colorIndex];

            // Blend this layer into our output buffer
            FT_Bitmap* bitmap = &m_face->glyph->bitmap;
            for (unsigned int y = 0; y < bitmap->rows; y++) {
                for (unsigned int x = 0; x < bitmap->width; x++) {
                    unsigned char alpha = bitmap->buffer[y * bitmap->pitch + x];
                    int destX = x + m_face->glyph->bitmap_left;
                    int destY = height - (y + m_face->glyph->bitmap_top);

                    if (destX >= 0 && destX < width && destY >= 0 && destY < height) {
                        size_t pixel = (destY * width + destX) * 4;

                        // Blend colors using alpha
                        float a = alpha / 255.0f;
                        bitmapVector[pixel + 0] = static_cast<unsigned char>(bitmapVector[pixel + 0] * (1 - a) + color.blue * a);
                        bitmapVector[pixel + 1] = static_cast<unsigned char>(bitmapVector[pixel + 1] * (1 - a) + color.green * a);
                        bitmapVector[pixel + 2] = static_cast<unsigned char>(bitmapVector[pixel + 2] * (1 - a) + color.red * a);
                        bitmapVector[pixel + 3] = static_cast<unsigned char>(std::max(bitmapVector[pixel + 3], alpha));
                    }
                }
            }
        }

        Texture charTexture;
        charTexture.setFromBitmapFlipped(
            bitmapVector.data(),
            width * 4,  // pitch is width * 4 for RGBA
            height,
            Texture::Format::RGBA
        );

        return m_characters.emplace(character, Char(character, std::move(charTexture),
            glm::ivec2(width, height),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y))).first->second;
    }

    const Font::Char& Font::getChar(char character) const {
        return (this->*m_getCharTable[static_cast<size_t>(m_type)])(character);
    }

    //coordinate shows left top letter
    void Font::drawTextAlignedLeft(std::string text, glm::vec2 coords,
        float scale, glm::vec3 color, Extent2D windowSize) const
    {
        Extent2D windowCoords = fromNDC(coords, windowSize);
        int x = windowCoords.width;
        int y = windowCoords.height;
        // activate corresponding render state	
        const Shader* shader;

        if (m_type != Type::NORMAL)
        {
            shader = &FTContext::instance().getShader(FTContext::Shaders::TEXT_COLORED);
            shader->bind();
        }
        else
        {
            shader = &FTContext::instance().getShader(FTContext::Shaders::TEXT);
            shader->bind();
            shader->setUniform3f("uColor", color.x, color.y, color.z);
        }

        shader->setUniformMat4f("uProjection", getWindowProjection(windowSize));
        shader->setUniform1f("uZValue", 0.0f);
        shader->setUniform1f("uScale", scale);

        glBindVertexArray(m_vao);

        // iterate through all characters
        for (int i = 0; i < text.size(); i++)
        {
            const Char& character = getChar(text[i]);
                        
            if(character.getChar() == '\n')
            {
                y -= (m_face->height >> 6) * scale;
                x = windowCoords.width;
                continue;
            }

            // render glyph texture over quad
            if(character.getTexture().getId() != 0)
            {
                character.getTexture().bind();

                shader->setUniform2f("uPosition", x, y);
                shader->setUniform2f("uSize", character.getSize().x, character.getSize().y);
                shader->setUniform2f("uBearing", character.getBearing().x, character.getBearing().y);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (character.getAdvance().x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //coordinate shows right top letter
    void Font::drawTextAlignedRight(std::string text, glm::vec2 coords,
        float scale, glm::vec3 color, Extent2D windowSize) const
    {
        Extent2D windowCoords = fromNDC(coords, windowSize);
        int x = windowCoords.width;
        int y = windowCoords.height;
        // activate corresponding render state	
        const Shader* shader;

        if (m_type != Type::NORMAL)
        {
            shader = &FTContext::instance().getShader(FTContext::Shaders::TEXT_COLORED);
            shader->bind();
        }
        else
        {
            shader = &FTContext::instance().getShader(FTContext::Shaders::TEXT);
            shader->bind();
            shader->setUniform3f("uColor", color.x, color.y, color.z);
        }

        shader->setUniformMat4f("uProjection", getWindowProjection(windowSize));
        shader->setUniform1f("uZValue", 0.0f);
        shader->setUniform1f("uScale", scale);

        glBindVertexArray(m_vao);

        //draw first line
        for (int i = 0; i < text.size(); i++)
        {
            const Char& character = getChar(text[i]);

            if (character.getChar() == '\n')
                break;
        }

        // iterate through all characters
        for (int i = 0; i < text.size(); i++)
        {
            const Char& character = getChar(text[i]);

            if (character.getChar() == '\n')
            {
                y -= (m_face->height >> 6) * scale;
                x = windowCoords.width;
                continue;
            }

            // render glyph texture over quad
            if (character.getTexture().getId() != 0)
            {
                character.getTexture().bind();

                shader->setUniform2f("uPosition", x, y);
                shader->setUniform2f("uSize", character.getSize().x, character.getSize().y);
                shader->setUniform2f("uBearing", character.getBearing().x, character.getBearing().y);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (character.getAdvance().x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}