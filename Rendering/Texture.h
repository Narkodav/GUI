#pragma once
#include <GL/glew.h>
#include <string>
#include <stdexcept>
#include <array>

#include "stb_image/stb_image.h"

class Texture
{
public:

    enum class Format
    {
        R,
        RG,
        RGB,
        RGBA,

        BGR,
        BGRA,

        RED_INTEGER,
        RG_INTEGER,
        RGB_INTEGER,
        RGBA_INTEGER,

        DEPTH_COMPONENT,
        DEPTH_STENCIL,

        STENCIL_INDEX,

        NUM
    };

    static inline const std::array<GLenum, static_cast<size_t>(Format::NUM)> m_formatBpps = {
        1,  // R
        2,  // RG
        3,  // RGB
        4,  // RGBA

        3,  // BGR
        4,  // BGRA

        1,  // RED_INTEGER
        2,  // RG_INTEGER
        3,  // RGB_INTEGER
        4,  // RGBA_INTEGER

        1,  // DEPTH_COMPONENT
        2,  // DEPTH_STENCIL

        1   // STENCIL_INDEX
    };

    static inline const std::array<GLenum, static_cast<size_t>(Format::NUM)> m_glFormatConversions = {
        GL_RED,
        GL_RG,
        GL_RGB,
        GL_RGBA,

        GL_BGR,
        GL_BGRA,

        GL_RED_INTEGER,
        GL_RG_INTEGER,
        GL_RGB_INTEGER,
        GL_RGBA_INTEGER,

        GL_DEPTH_COMPONENT,
        GL_DEPTH_STENCIL,

        GL_STENCIL_INDEX
    };

    static inline const std::array<GLenum, static_cast<size_t>(Format::NUM)> m_glInternalFormatConversions = {
    GL_R8,           // R
    GL_RG8,          // RG
    GL_RGB8,         // RGB
    GL_RGBA8,        // RGBA

    GL_RGB8,         // BGR
    GL_RGBA8,        // BGRA

    GL_R8I,          // RED_INTEGER
    GL_RG8I,         // RG_INTEGER
    GL_RGB8I,        // RGB_INTEGER
    GL_RGBA8I,       // RGBA_INTEGER

    GL_DEPTH_COMPONENT24,  // DEPTH_COMPONENT
    GL_DEPTH24_STENCIL8,  // DEPTH_STENCIL

    GL_STENCIL_INDEX8     // STENCIL_INDEX
    };

private:
	unsigned int m_id;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_bpp;
public:
	Texture();
	Texture(const std::string& path);
	Texture(unsigned char* bitmap, int width, int height, Format format);
	~Texture();

    Texture(const Texture& other)
    {
        m_id = 0;
        m_filePath = other.m_filePath;
        m_localBuffer = other.m_localBuffer;
        m_width = other.m_width;
        m_height = other.m_height;
        m_bpp = other.m_bpp;

        if (other.m_id == 0)
            return;

        // Generate new texture ID
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        // Set the same parameters as the source texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Allocate storage for the new texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glCopyImageSubData(
            other.m_id, GL_TEXTURE_2D, 0, 0, 0, 0,
            m_id, GL_TEXTURE_2D, 0, 0, 0, 0,
            m_width, m_height, 1
        );
    }

    Texture& operator=(const Texture& other)
    {
        if (this == &other)
            return *this;

        clear();

        m_id = 0;
        m_filePath = other.m_filePath;
        m_localBuffer = other.m_localBuffer;
        m_width = other.m_width;
        m_height = other.m_height;
        m_bpp = other.m_bpp;

        if(other.m_id == 0)
            return *this;

        // Generate new texture ID
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        // Set the same parameters as the source texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Allocate storage for the new texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glCopyImageSubData(
            other.m_id, GL_TEXTURE_2D, 0, 0, 0, 0,
            m_id, GL_TEXTURE_2D, 0, 0, 0, 0,
            m_width, m_height, 1
        );
        return *this;
    }

    Texture(Texture&& other) noexcept
    {
        m_id = std::exchange(other.m_id, 0);
        m_filePath = std::exchange(other.m_filePath, "none");
        m_localBuffer = std::exchange(other.m_localBuffer, nullptr);
        m_width = std::exchange(other.m_width, 0);
        m_height = std::exchange(other.m_height, 0);
        m_bpp = std::exchange(other.m_bpp, 0);
    }

    Texture& operator=(Texture&& other) noexcept
    {
        if (this == &other)
            return *this;
        m_id = std::exchange(other.m_id, 0);
        m_filePath = std::exchange(other.m_filePath, "none");
        m_localBuffer = std::exchange(other.m_localBuffer, nullptr);
        m_width = std::exchange(other.m_width, 0);
        m_height = std::exchange(other.m_height, 0);
        m_bpp = std::exchange(other.m_bpp, 0);
        return *this;
    }

	bool operator==(Texture& other) const noexcept; //compares ids

	void setFromBitmap(unsigned char* bitmap, int width, int height, Format format);
    void setFromBitmapFlipped(unsigned char* bitmap, int width, int height, Format format);
	void load(const std::string& path);
	void clear();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return m_width; };
	inline int getHeight() const { return m_height; };
	inline unsigned int getId() const { return m_id; };
};
