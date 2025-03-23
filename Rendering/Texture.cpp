#include "Texture.h"


Texture::Texture() :
	m_id(0), m_localBuffer(nullptr), m_width(0), m_height(0), m_bpp(0), m_filePath("none")
{
	
}

Texture::Texture(const std::string& path) :
	m_id(0), m_localBuffer(nullptr), m_width(0), m_height(0), m_bpp(0), m_filePath("none")
{
	load(path);
}

Texture::Texture(unsigned char* bitmap, int width, int height, Format format) :
	m_id(0), m_localBuffer(nullptr), m_width(0), m_height(0), m_bpp(0), m_filePath("none")
{
	setFromBitmap(bitmap, width, height, format);
}


Texture::~Texture()
{
	if (m_id)
		clear();
}

bool Texture::operator==(Texture& other) const noexcept {
	return this->m_id == other.m_id;

};

void Texture::load(const std::string& path)
{
	if (m_id)
		clear();
	m_filePath = path;
	stbi_set_flip_vertically_on_load(1);
	m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

	if (m_localBuffer)
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(m_localBuffer);
	}
	else throw std::runtime_error("Failed to load texture: " + path);
}

void Texture::setFromBitmap(unsigned char* bitmap, int width, int height, Format format)
{
	if (m_id)
		clear();
	m_filePath = "none";
	m_width = width;
	m_height = height;
	m_localBuffer = bitmap;
	m_bpp = m_formatBpps[static_cast<size_t>(format)];

	if (m_localBuffer)
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(m_bpp == 1)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, m_glInternalFormatConversions[static_cast<size_t>(format)],
				m_width, m_height, 0, m_glFormatConversions[static_cast<size_t>(format)],
				GL_UNSIGNED_BYTE, m_localBuffer);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, m_glInternalFormatConversions[static_cast<size_t>(format)],
				m_width, m_height, 0, m_glFormatConversions[static_cast<size_t>(format)],
				GL_UNSIGNED_BYTE, m_localBuffer);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else throw std::runtime_error("bitmap is null");
}

void Texture::setFromBitmapFlipped(unsigned char* bitmap, int width, int height, Format format)
{	
	if (m_id)
		clear();
	m_filePath = "none";
	m_width = width;
	m_height = height;
	m_localBuffer = nullptr;
	m_bpp = m_formatBpps[static_cast<size_t>(format)];

	if (bitmap)
	{
		// Create a temporary buffer for the flipped image
		size_t rowSize = width * m_bpp;
		m_localBuffer = new unsigned char[width * height * m_bpp];

		// Flip vertically by copying rows in reverse order
		for (int y = 0; y < height; y++) {
			memcpy(
				m_localBuffer + y * rowSize,                    // destination row
				bitmap + (height - 1 - y) * rowSize,           // source row (flipped)
				rowSize
			);
		}

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if (m_bpp == 1)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, m_glInternalFormatConversions[static_cast<size_t>(format)],
				m_width, m_height, 0, m_glFormatConversions[static_cast<size_t>(format)],
				GL_UNSIGNED_BYTE, m_localBuffer);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, m_glInternalFormatConversions[static_cast<size_t>(format)],
				m_width, m_height, 0, m_glFormatConversions[static_cast<size_t>(format)],
				GL_UNSIGNED_BYTE, m_localBuffer);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		delete[] m_localBuffer;
	}
	else throw std::runtime_error("bitmap is null");
}

void Texture::clear()
{
	glDeleteTextures(1, &m_id);
	m_id = 0;
}

void Texture::bind(unsigned int slot /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
