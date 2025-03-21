#pragma once
#include "Board.h"
#include "Mouse.h"
#include "AssetRepository.h"
#include "FlatTexture.h"
#include "FrameBuffer.h"

class Renderer
{
	AssetRepository m_assets;
	
	FlatTexture m_boardMesh;
	bool m_shouldHighlight = false;
	FlatTexture m_highlightMesh;
	FrameBuffer m_frameBuffer;
	glm::mat4 m_projection;

	//highlight values
	float m_highlightTime = 0.0f;
	float m_baseAlpha = 0.8f;  // Maximum alpha
	float m_pulseSpeed = 3.0f;  // Adjust this to make it faster/slower
	float m_pulseAmount = 0.3f; // How much it pulses (0.4 means it goes from 0.4 to 0.8)

	std::vector<FlatTexture> m_pieces;
	std::vector<FlatTexture> m_moveHighlights;

public:

	void draw(const Board& board, const Mouse& mouse,
		GLFWwindow* windowHandle, size_t windowWidth,
		size_t windowHeight, float deltaTime);

	void loadAssets() { 
		m_assets.loadAssets();
		m_frameBuffer.initializeFramebuffer(Board::BOARD_WIDTH, Board::BOARD_HEIGHT,
			&m_assets.getShader(AssetRepository::Shaders::FLAT_TO_SCREEN));
		m_projection = glm::ortho(0.0f, (float)Board::BOARD_WIDTH,
			(float)Board::BOARD_HEIGHT, 0.0f, -1.0f, 1.0f);
	};

	void remeshScene(const Board& board, size_t windowWidth, size_t windowHeight);
	void setHighlight(const Board& board, const Mouse& mouse, size_t windowWidth, size_t windowHeight);

private:
	void drawBoard();
	void drawPieces();
	void drawHighlight();
	void drawMoveHighlights();
};

