#pragma once

#include <glm/glm.hpp>

struct Node
{
	glm::mat4 mTransform{ glm::mat4(1) };

	virtual ~Node() = default;
};
