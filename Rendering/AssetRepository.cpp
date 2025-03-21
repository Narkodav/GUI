#include "AssetRepository.h"

void AssetRepository::loadAssets()
{
	for (size_t i = 0; i < m_textures.size(); i++)
		m_textures[i].load("Assets/" + textureNames[i]);

	for (size_t i = 0; i < m_shaders.size(); i++)
		m_shaders[i].set("Shaders/" + shaderNames[i]);
}