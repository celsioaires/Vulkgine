#pragma once

#include "../Render/Mesh.h"

struct Geometry
{
	uint32_t mStartIndex;
	uint32_t mIndexCount;
};

struct MeshAsset
{
	Mesh mMesh;
	std::vector<Geometry> mGeometries;
};
