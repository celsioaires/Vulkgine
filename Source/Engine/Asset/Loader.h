#pragma once

#include <filesystem>

#include "MeshAsset.h"
#include "TextureAsset.h"

class Renderer;

struct Loader
{
	static std::vector<MeshAsset> loadMeshes(Renderer& renderer, std::filesystem::path path);
	static TextureAsset loadTexture(Renderer& renderer, std::filesystem::path path);
};
