#pragma once

#include <filesystem>

#include "MeshAsset.h"

class Renderer;

struct Loader
{
	static std::vector<MeshAsset> loadMeshes(Renderer& renderer, std::filesystem::path path);
};
