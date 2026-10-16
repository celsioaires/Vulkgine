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
	Loader::unloadGltf(mAsset);
}

void Scene::submitRenderables(Renderer& renderer)
{
	for (Node* node : mAsset.mNodes)
	{
		auto* meshNode = dynamic_cast<MeshNode*>(node);

		if (meshNode)
		{
			// Data
			MeshAsset& meshAsset = meshNode->mAsset;
			glm::mat4 topMatrix(1);
			glm::mat4 nodeMatrix = topMatrix * meshNode->mWorldTransform;

			// Renderables
			for (Geometry& geometry : meshAsset.mGeometries)
			{
				Renderable renderable{};

				renderable.mMesh = meshAsset.mGpuData;
				renderable.mGeometry = geometry;
				renderable.mTransform = nodeMatrix;

				bool transparent{};

				if (geometry.mMaterial.mType == MaterialType::Transparent)
					transparent = true;

				renderer.submitRenderable(renderable, transparent);
			}
		}
		// TODO: else if (otherNode)
	}
}
