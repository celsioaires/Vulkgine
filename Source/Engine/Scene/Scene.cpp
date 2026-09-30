#include "Scene.h"

#include <array>

#include "../Asset/Loader.h"

#include "MeshNode.h"

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
void Scene::initializeNodes()
{
	for (MeshAsset& meshAsset : mMeshAssets)
	{
		auto* meshNode = new MeshNode{};
		meshNode->mAsset = meshAsset;

		mNodes.push_back(meshNode);
	}
}

void Scene::cleanupInitialized()
{
	for (Node* node : mNodes)
		delete node;

	mNodes.clear();

	for (TextureAsset& textureAsset : mTextureAssets)
		textureAsset.mGpuData.cleanupInitialized();

	for (MeshAsset& meshAsset : mMeshAssets)
		meshAsset.mGpuData.cleanupInitialized();
}

void Scene::submitRenderables(Renderer& renderer)
{
	for (Node* node : mNodes)
	{
		auto* meshNode = dynamic_cast<MeshNode*>(node);

		if (meshNode)
		{
			MeshAsset& meshAsset = meshNode->mAsset;

			for (Geometry& meshGeometry : meshAsset.mGeometries)
			{
				Renderable renderable{};
				renderable.mMesh = meshAsset.mGpuData;
				renderable.mGeometry = meshGeometry;
				renderable.mTransform = node->mTransform;

				renderer.submitRenderable(renderable);
			}
		}
	}
}
