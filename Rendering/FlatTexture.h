#pragma once
#include "AssetRepository.h"

class FlatTexture
{
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

	enum class Corners
	{
		LEFT_DOWN,
		RIGHT_DOWN,
		RIGHT_UP,
		LEFT_UP,
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

private:
	std::array<glm::vec2, 4> m_vertices;
	float m_zValue = 0.0f;

	std::array<uint32_t, static_cast<size_t>(Buffers::NUM)> m_buffers;
	uint32_t m_vao;

	const Texture* m_textureHandle = nullptr; //optional

	bool m_isSet = false;
public:

	FlatTexture() : m_vao(0), m_textureHandle(nullptr) {
		m_vertices.fill(glm::vec2(0.0f));
		m_buffers.fill(0);
	};

	~FlatTexture() {
		if (m_isSet)
			deleteBuffers();
	};

	FlatTexture(FlatTexture&&) noexcept = default;
	FlatTexture& operator=(FlatTexture&&) noexcept = default;

	FlatTexture(const FlatTexture&) = delete;
	FlatTexture& operator=(const FlatTexture&) = delete;

	FlatTexture& setTexture(const Texture* texture)
	{
		m_textureHandle = texture;
		return *this;
	}

	void copyVertices(const FlatTexture& texture)
	{
		m_vertices = texture.m_vertices;
	}

	FlatTexture& setVertices(const std::array<glm::vec2, 4>& vertices) {
		m_vertices = vertices;
		return *this;
	};

	FlatTexture& setZValue(float zValue) {
		m_zValue = zValue;
		return *this;
	};

	FlatTexture& setFromRect(const Rectangle& rect, size_t windowWidth, size_t windowHeight) {

		// Calculate shared X coordinates
		float leftX = (2.0f * rect.offsetX) / windowWidth - 1.0f;
		float rightX = (2.0f * (rect.offsetX + rect.width)) / windowWidth - 1.0f;

		// Calculate shared Y coordinates
		float topY = 1.0f - (2.0f * rect.offsetY) / windowHeight;
		float bottomY = 1.0f - (2.0f * (rect.offsetY + rect.height)) / windowHeight;

		// Assign vertices
		m_vertices[static_cast<size_t>(Corners::LEFT_DOWN)].x = leftX;
		m_vertices[static_cast<size_t>(Corners::LEFT_DOWN)].y = bottomY;

		m_vertices[static_cast<size_t>(Corners::LEFT_UP)].x = leftX;
		m_vertices[static_cast<size_t>(Corners::LEFT_UP)].y = topY;

		m_vertices[static_cast<size_t>(Corners::RIGHT_DOWN)].x = rightX;
		m_vertices[static_cast<size_t>(Corners::RIGHT_DOWN)].y = bottomY;

		m_vertices[static_cast<size_t>(Corners::RIGHT_UP)].x = rightX;
		m_vertices[static_cast<size_t>(Corners::RIGHT_UP)].y = topY;

		return *this;
	};

	FlatTexture& setupBuffers()
	{
		if (m_isSet)
			deleteBuffers();
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(static_cast<size_t>(Buffers::NUM), m_buffers.data());

		// Bind VAO first
		glBindVertexArray(m_vao);

		// Setup position buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[static_cast<size_t>(Buffers::POSITION)]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);
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
		m_isSet = true;
		return *this;
	}

	float getZValue() const { return m_zValue; };

	void deleteBuffers()
	{
		glDeleteBuffers(static_cast<size_t>(Buffers::NUM), m_buffers.data());
		glDeleteVertexArrays(1, &m_vao);
		m_isSet = false;
	}

	FlatTexture& setupBuffersNoTexture() //in case the texture is hard coded in the shader
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(static_cast<size_t>(Buffers::NUM), m_buffers.data());

		// Bind VAO first
		glBindVertexArray(m_vao);

		// Setup position buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[static_cast<size_t>(Buffers::POSITION)]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(static_cast<size_t>(Locations::POSITION), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(static_cast<size_t>(Locations::POSITION));

		// Setup element buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[static_cast<size_t>(Buffers::INDEX)]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

		// Unbind VAO
		glBindVertexArray(0);
	}

	void draw() const
	{
		if (m_textureHandle != nullptr)
			m_textureHandle->bind();
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};

