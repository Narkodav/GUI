#pragma once
#include "Common.h"
#include "TextureAtlas.h"
#include "Texture.h"
#include "Shader.h"

class AssetRepository
{
public:

    //matches the Piece enum exactly
	enum class Textures : size_t
	{
        TEXTURE_BOARD,

        // White pieces
        TEXTURE_WHITE_PAWN,
        TEXTURE_WHITE_KNIGHT,
        TEXTURE_WHITE_BISHOP,
        TEXTURE_WHITE_ROOK,
        TEXTURE_WHITE_QUEEN,
        TEXTURE_WHITE_KING,

        // Black pieces
        TEXTURE_BLACK_PAWN,
        TEXTURE_BLACK_KNIGHT,
        TEXTURE_BLACK_BISHOP,
        TEXTURE_BLACK_ROOK,
        TEXTURE_BLACK_QUEEN,
        TEXTURE_BLACK_KING,

        TEXTURE_COUNT
	};

    enum class Shaders
    {
        FLAT_TO_SCREEN,
        HIGHLIGHT,
        FLAT_TO_SCREEN_WITH_PROJECTION,
        SHADER_COUNT,
    };

    static inline const std::array<std::string, static_cast<size_t>(Textures::TEXTURE_COUNT)> textureNames = {
            "board.png",

            // White pieces
            "white_pawn.png",
            "white_knight.png",
            "white_bishop.png",
            "white_rook.png",
            "white_queen.png",
            "white_king.png",

            // Black pieces
            "black_pawn.png",
            "black_knight.png",
            "black_bishop.png",
            "black_rook.png",
            "black_queen.png",
            "black_king.png"
    };

    static inline const std::array<std::string, static_cast<size_t>(Shaders::SHADER_COUNT)> shaderNames = {
            "FlatToScreen.shader",
            "Highlight.shader",
            "FlatToScreenWithProjection.shader",
    };

    // might make these configurable
    static inline const size_t boardWidth = 142;
    static inline const size_t boardHight = 142;

    static inline const size_t framingWidth = 7;
    static inline const size_t cellWidth = 16;
    static inline const size_t cellHight = 16;

    static inline const size_t pieceWidth = 16;
    static inline const size_t pieceHight = 16;

    //// Medium gray with slight blue tint
    //glClearColor(0.2f, 0.2f, 0.25f, 1.0f);

    //// Darker gray-blue
    //glClearColor(0.15f, 0.15f, 0.2f, 1.0f);

    //// Navy-like background (darker)
    //glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    static inline const glm::vec4 backgroundColor = { 0.1f, 0.1f, 0.15f, 1.0f };

private:
	TextureAtlas m_atlas;
	std::array<Texture, static_cast<size_t>(Textures::TEXTURE_COUNT)> m_textures;
    std::array<Shader, static_cast<size_t>(Shaders::SHADER_COUNT)> m_shaders;

public:
	void loadAssets();

    const Texture& getTexture(Textures texture) { return m_textures[static_cast<size_t>(texture)]; };
    const Shader& getShader(Shaders shader) { return m_shaders[static_cast<size_t>(shader)]; };
};

