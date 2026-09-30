#pragma once

#include "../Render/Renderer.h"
#include "../Asset/MeshAsset.h"
#include "../Asset/TextureAsset.h"

#include "Node.h"

class Scene
{
private:
	std::vector<MeshAsset> mMeshAssets{};
	std::vector<TextureAsset> mTextureAssets{};
	std::vector<Node*> mNodes{};
public:
	void initializeMeshes(Renderer& renderer);
	void initializeTextures(Renderer& renderer);
	void initializeNodes();

	void cleanupInitialized();

	void submitRenderables(Renderer& renderer);

	std::vector<TextureAsset> getTextures() { return mTextureAssets; }
	std::vector<Node*>& getNodes() { return mNodes; }
};
