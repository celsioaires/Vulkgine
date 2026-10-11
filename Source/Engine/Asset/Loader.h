#pragma once

#include <filesystem>

#include "GltfAsset.h"
#include "TextureAsset.h" // TODO: remove?

namespace fastgltf { class Asset; }

class Renderer;

struct Loader
{
	static GltfAsset loadGltf(Renderer& renderer, std::filesystem::path path);

	// Gltf loaders
	static std::vector<MaterialAsset> loadMaterials(Renderer& renderer, fastgltf::Asset& asset);
	static std::vector<MeshAsset> loadMeshes(Renderer& renderer, fastgltf::Asset& asset, std::vector<MaterialAsset>& materials);
	static std::vector<Node*> loadNodes(fastgltf::Asset& asset, std::vector<MeshAsset>& meshes);
	static std::vector<Node*> loadTopNodes(std::vector<Node*>& nodes);

	// TODO: remove?
	static TextureAsset loadTexture(Renderer& renderer, std::filesystem::path path, int texture);
};
