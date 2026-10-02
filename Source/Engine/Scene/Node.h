#pragma once

#include <string>

#include <glm/glm.hpp>

struct Node
{
	std::string mName{};
	glm::mat4 mTransform{ glm::mat4(1) };

	virtual ~Node() = default;
};
