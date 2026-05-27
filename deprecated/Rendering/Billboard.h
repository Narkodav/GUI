#pragma once
#include <GL/glew.h>
#include <string>
#include <array>

#include "Shader.h"
#include "Shaders/ShaderCache.h"
#include "Texture.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Billboard
{
public:
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
    GLuint m_vao = 0;
    std::array<GLuint, static_cast<size_t>(Buffers::NUM)> m_buffers = { 0 };
    std::array<glm::vec2, 4> m_positions = {
        glm::vec2(-1.0f, -1.0f),
        glm::vec2(1.0f, -1.0f),
        glm::vec2(1.0f,  1.0f),
        glm::vec2(-1.0f,  1.0f),
    };

public:

    Billboard(glm::vec2 topLeft, glm::vec2 bottomRight) {
        m_positions[0] = glm::vec2(topLeft.x, bottomRight.y);
        m_positions[1] = bottomRight;
        m_positions[2] = glm::vec2(bottomRight.x, topLeft.y);
        m_positions[3] = topLeft;

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(static_cast<size_t>(Buffers::NUM), m_buffers.data());

        // Bind VAO first
        glBindVertexArray(m_vao);

        // Setup position buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_buffers[static_cast<size_t>(Buffers::POSITION)]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_positions.size(), m_positions.data(), GL_STATIC_DRAW);
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

    Billboard(const Billboard&) = delete;
    Billboard(Billboard&&) = default;
    Billboard& operator=(const Billboard&) = delete;
    Billboard& operator=(Billboard&&) = default;

    ~Billboard() {
        glDeleteBuffers(static_cast<size_t>(Buffers::NUM), m_buffers.data());
        glDeleteVertexArrays(1, &m_vao);
    }

    inline void drawToScreen(const glm::mat4& projection, float zValue, GLuint texture) const {
        // Bind framebuffer and set viewport
        const Shader& shader = ShaderCache::getInstance().getShader<ShaderCache::ShaderSpecialisation::FLAT_TO_SCREEN_PROJECTION>();
        shader.bind();
        shader.setUniformMat4f("uProjection", projection);
        shader.setUniform1f("uZValue", zValue);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

   inline GLuint getBuffer(Buffers buffer) const {
       return m_buffers[static_cast<size_t>(buffer)];
   };

   inline GLuint getVAO() const {
       return m_vao;
   };
};

