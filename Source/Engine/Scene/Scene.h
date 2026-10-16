#pragma once

#include "../Render/Renderer.h"
#include "../Asset/GltfAsset.h"

#include "Node.h"

class Scene
{
private:
	GltfAsset mAsset{};
public:
	void initializeAsset(Renderer& renderer);
	void cleanupInitialized();

	void submitRenderables(Renderer& renderer);

	std::vector<Node*>& getNodes() { return mAsset.mNodes; }
};
