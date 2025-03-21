#pragma once
#include <GL/glew.h>
#include <string>
#include <array>

#include "Shader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class FrameBuffer
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

    static inline const std::array<glm::vec2, 4> positions = {
        glm::vec2(-1.0f, -1.0f),
        glm::vec2( 1.0f, -1.0f),
        glm::vec2( 1.0f,  1.0f),
        glm::vec2(-1.0f,  1.0f),
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
    GLuint m_frameBuffer;
    GLuint m_renderTexture;
    GLuint m_depthRenderBuffer;

    size_t m_width, m_height;
    size_t m_offsetX, m_offsetY;
    GLuint m_vao;
    std::array<GLuint, static_cast<size_t>(Buffers::NUM)> m_buffers;
    const Shader* m_shader;

    glm::mat4 m_projection;

    bool m_isInit = false;
public:
    FrameBuffer() : m_width(0), m_height(0), m_shader(nullptr), m_isInit(false) {};

    ~FrameBuffer() {
        if (m_isInit)
            deleteBuffers();
    };

    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;

    FrameBuffer(FrameBuffer&& other) noexcept = default;
    FrameBuffer& operator=(FrameBuffer&& other) noexcept = default;


    void bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
        glViewport(0, 0, m_width, m_height);  // Set viewport to framebuffer size
    }

    void unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // can handle negative coordinates
    // coords is coordinates of top left corner in pixels, width and hight are dimensions of the result
    void drawToScreen(int coordsX, int CoordsY, int width, int height,
        int screenWidth, int screenHeight) {
        // Calculate visible region
        int visibleX = std::max(0, coordsX);
        int visibleY = std::max(0, CoordsY);
        int visibleWidth = width - visibleX + coordsX;
        int visibleHeight = height - visibleY + CoordsY;

        // Calculate texture coordinates for the visible portion
        float texStartX = static_cast<float>(-coordsX) / width;
        float texStartY = static_cast<float>(-CoordsY) / height;
        float texEndX = texStartX + static_cast<float>(visibleWidth) / width;
        float texEndY = texStartY + static_cast<float>(visibleHeight) / height;

        // Update UV coordinates for the visible portion
        std::array<glm::vec2, 4> adjustedUVs = {
            glm::vec2(texStartX, texStartY),  // bottom-left
            glm::vec2(texEndX, texStartY),    // bottom-right
            glm::vec2(texEndX, texEndY),      // top-right
            glm::vec2(texStartX, texEndY)     // top-left
        };

        // Enable scissor test to clip rendering
        glEnable(GL_SCISSOR_TEST);
        glScissor(visibleX, visibleY, visibleWidth, visibleHeight);

        // Bind framebuffer and set viewport
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(visibleX, visibleY, visibleWidth, visibleHeight);

        m_shader->bind();
        m_shader->setUniformMat4f("uProjection", m_projection);
        m_shader->setUniform1f("uZValue", 0.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_renderTexture);

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    GLuint getTextureId() const { return m_renderTexture; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    void init(int width, int height, const Shader* shader) {
        if (m_isInit)
            deleteBuffers();

        m_width = width;
        m_height = height;
        m_shader = shader;

        m_projection = glm::ortho(static_cast<float>(0),
            static_cast<float>(width),
            static_cast<float>(0),
            static_cast<float>(height));

        // Create framebuffer
        glGenFramebuffers(1, &m_frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

        glGenRenderbuffers(1, &m_depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER, m_depthRenderBuffer);

        // Create texture to render to
        glGenTextures(1, &m_renderTexture);
        glBindTexture(GL_TEXTURE_2D, m_renderTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
            m_width, m_height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, m_renderTexture, 0);

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

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Framebuffer is not complete!");
        }

        m_isInit = true;
    }

    void deleteBuffers()
    {
        glDeleteBuffers(static_cast<size_t>(Buffers::NUM), m_buffers.data());
        glDeleteVertexArrays(1, &m_vao);
        glDeleteTextures(1, &m_renderTexture);
        glDeleteFramebuffers(1, &m_frameBuffer);

        m_isInit = false;
    }

    void clear()
    {
        bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        unbind();  
    }
};

