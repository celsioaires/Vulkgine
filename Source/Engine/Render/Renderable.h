#pragma once

#include "Mesh.h"
#include "Geometry.h"

struct Renderable
{
	Mesh mMesh{};
	Geometry mGeometry{};
	glm::mat4 mTransform{ glm::mat4(1) };
};
	