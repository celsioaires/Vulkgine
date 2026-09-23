#pragma once

#include "../Render/Mesh.h"
#include "../Render/Renderer.h"
#include "../Asset/MeshAsset.h"
#include "../Asset/TextureAsset.h"

class Scene
{
private:
	std::vector<MeshAsset> mMeshAssets;
	std::vector<TextureAsset> mTextureAssets;
public:
	void initializeMeshes(Renderer& renderer);
	void initializeTextures(Renderer& renderer);

	void cleanupInitialized();

	// Getters
	std::vector<MeshAsset> getMeshes() { return mMeshAssets; }
};
