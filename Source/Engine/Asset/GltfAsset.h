#pragma once

#include "../Scene/Node.h"
#include "TextureAsset.h"
#include "MaterialAsset.h"
#include "MeshAsset.h"

struct GltfAsset
{
	std::vector<TextureAsset> mTextures{};
	std::vector<MaterialAsset> mMaterials{};
	std::vector<MeshAsset> mMeshes{};
	std::vector<Node*> mNodes{};
	std::vector<Node*> mTopNodes{};

	~GltfAsset()
	{

	}
};
