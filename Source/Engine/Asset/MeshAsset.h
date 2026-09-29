#pragma once

#include "../Render/Mesh.h"
#include "../Render/Geometry.h"

struct MeshAsset
{
	Mesh mGpuData{};
	std::vector<Geometry> mGeometries{};
};
