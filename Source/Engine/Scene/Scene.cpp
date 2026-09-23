#include "Scene.h"

#include <array>

#include "../Asset/Loader.h"

void Scene::initializeMeshes(Renderer& renderer)
{
	mMeshAssets = Loader::loadMeshes(renderer, "Assets/Models/basicmesh.glb");
}

void Scene::initializeTextures(Renderer& renderer)
{
	std::string directory = "Assets/Textures/";

	std::string files[]
	{
		".jpg"
	};

	for (std::string file : files)
	{
		TextureAsset asset = Loader::loadTexture(renderer, directory + file);
		mTextureAssets.emplace_back(std::move(asset));
	}
}

void Scene::cleanupInitialized()
{
	for (TextureAsset& asset : mTextureAssets)
		asset.mGpuData.cleanupInitialized();

	for (MeshAsset& asset : mMeshAssets)
		asset.mGpuData.cleanupInitialized();
}
