#pragma once

#include <string>

#include "../Render/Mesh.h"
#include "../Render/Geometry.h"

struct MeshAsset
{
	std::string mName{};
	Mesh mGpuData{};
	std::vector<Geometry> mGeometries{};
};
