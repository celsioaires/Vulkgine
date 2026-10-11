#pragma once

#include "../Render/Material.h"

struct MaterialAsset
{
	MaterialConstants mConstants{};
	Material mGpuData{};
};
