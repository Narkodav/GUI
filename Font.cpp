#include "Font.h"

namespace GUI
{

	Font::Font(const std::string& path)
	{
        FTContext::instance();
		if (FT_New_Face(FTContext::instance(), path.c_str(), 0, &m_face))
			throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
		m_isColor = (m_face->face_flags & FT_FACE_FLAG_COLOR) != 0;

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


    void Font::setCharSize(int width, int height)
    {
        m_width = width;
        m_height = height;
        if (FT_Set_Pixel_Sizes(m_face, m_width, m_height))
            throw std::runtime_error("ERROR::FREETYPE: Failed to set pixel sizes");
    }

    void Font::setCharSize(int height) //automatically deduces width
    {
        m_width = 0;
        m_height = height;
        if (FT_Set_Pixel_Sizes(m_face, m_width, m_height))
            throw std::runtime_error("ERROR::FREETYPE: Failed to set pixel sizes");
    }

    void Font::renderCharMap()
    {
        m_characters.clear();
        for (int i = 0; i < 256; i++)
        {
            m_characters.emplace(i, getChar(i));
        }    
    }

    Font::Char Font::getChar(char character) {
        Texture charTexture;
        if (m_isColor)
        {
            if (FT_Load_Char(m_face, character, FT_LOAD_COLOR))
                throw std::runtime_error("ERROR::FREETYPE: Failed to load a character");

            if (!m_face->glyph->bitmap.buffer)
                return Char(character, Texture(),
                    glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                    glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                    glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y));

            charTexture.setFromBitmap(
                m_face->glyph->bitmap.buffer,
                m_face->glyph->bitmap.width,
                m_face->glyph->bitmap.rows,
                Texture::Format::BGRA //colored
            );
        }
        else
        {
            if (FT_Load_Char(m_face, character, FT_LOAD_RENDER))
                throw std::runtime_error("ERROR::FREETYPE: Failed to load a character");

            if (!m_face->glyph->bitmap.buffer)
                return Char(character, Texture(),
                    glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                    glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                    glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y));

            charTexture.setFromBitmap(
                m_face->glyph->bitmap.buffer,
                m_face->glyph->bitmap.width,
                m_face->glyph->bitmap.rows,
                Texture::Format::R //grayscale
            );
        }

        return Char(character, std::move(charTexture),
            glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            glm::ivec2(m_face->glyph->advance.x, m_face->glyph->advance.y));
    }

    void Font::drawText(std::string text, glm::vec2 coords,
        float scale, glm::vec4 color, Extent2D windowSize) const
    {
        Extent2D windowCoords = fromNDC(coords, windowSize);
        int x = windowCoords.width;
        int y = windowCoords.height;
        // activate corresponding render state	
        const auto& shader = FTContext::instance().getShader();

        shader.bind();
        shader.setUniform4f("uColor", color.x, color.y, color.z, color.w);
        shader.setUniformMat4f("uProjection", getWindowProjection(windowSize));
        shader.setUniform1f("uZValue", 0.0f);
        shader.setUniform1f("uScale", scale);

        glBindVertexArray(m_vao);

        // iterate through all characters
        for (int i = 0; i < text.size(); i++)
        {
            const auto& ch = m_characters.find(text[i]);
            if (ch == m_characters.end())
                throw std::runtime_error("invalid character");
            auto& character = ch->second;
                        
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

                shader.setUniform2f("uPosition", x, y);
                shader.setUniform2f("uSize", character.getSize().x, character.getSize().y);
                shader.setUniform2f("uBearing", character.getBearing().x, character.getBearing().y);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (character.getAdvance().x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}