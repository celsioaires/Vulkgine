#include "Scene.h"

#include <array>

#include <fmt/core.h>

#include "../Asset/Loader.h"
#include "MeshNode.h"

void Scene::initializeAsset(Renderer& renderer)
{
	mAsset = Loader::loadGltf(renderer, "Assets/Models/structure.glb");
}

void Scene::cleanupInitialized()
{

}

void Scene::submitRenderables(Renderer& renderer)
{
	for (Node* node : mAsset.mNodes)
	{
		auto* meshNode = dynamic_cast<MeshNode*>(node);

		if (meshNode)
		{
			MeshAsset& meshAsset = meshNode->mAsset;
			glm::mat4 topMatrix(1);
			glm::mat4 nodeMatrix = topMatrix * meshNode->mWorldTransform;

			for (Geometry& meshGeometry : meshAsset.mGeometries)
			{
				Renderable renderable{};
				renderable.mMesh = meshAsset.mGpuData;
				renderable.mGeometry = meshGeometry;
				renderable.mTransform = nodeMatrix;

				renderer.submitRenderable(renderable);
			}
		}
		// TODO: else if (otherNode)
	}
}
