#pragma once

#include "Node.h"

#include "../Asset/MeshAsset.h"

struct MeshNode : public Node
{
	MeshAsset mAsset{};

};
