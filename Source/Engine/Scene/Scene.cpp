#include "Scene.h"

#include <array>

#include "../Asset/Loader.h"

void Scene::initializeMeshes(Renderer& renderer)
{
	std::array<Vertex, 4> vertices;

	vertices[0].mPosition = { 0.5,-0.5, 0 };
	vertices[1].mPosition = { 0.5,0.5, 0 };
	vertices[2].mPosition = { -0.5,-0.5, 0 };
	vertices[3].mPosition = { -0.5,0.5, 0 };

	std::array<uint32_t, 6> indices;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	mMesh.initializeBuffers(renderer, vertices, indices);

	mMeshAssets = Loader::loadMeshes(renderer, "Assets/Models/basicmesh.glb");
}

void Scene::cleanupInitialized()
{
	for (MeshAsset& mesh : mMeshAssets)
		mesh.mMesh.cleanupInitialized();

	mMesh.cleanupInitialized();
}
