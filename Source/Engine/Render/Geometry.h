#pragma once

#include <cstdint>

#include "Material.h"

struct Geometry
{
	uint32_t mStartIndex{};
	uint32_t mIndexCount{};
	Material mMaterial{};
};
