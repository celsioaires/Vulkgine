#pragma once

#include "../Render/Mesh.h"
#include "../Render/Renderer.h"
#include "../Asset/MeshAsset.h"

class Scene
{
private:
	Mesh mMesh;
	std::vector<MeshAsset> mMeshAssets;
public:
	void initializeMeshes(Renderer& renderer);

	void cleanupInitialized();

	// Getters
	std::vector<MeshAsset> getMeshes() { return mMeshAssets; }
	Mesh getMesh() { return mMesh; }
};
