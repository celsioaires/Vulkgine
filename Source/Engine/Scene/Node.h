#pragma once

#include <string>

#include <glm/glm.hpp>

struct Node
{
	std::string mName{ "Unnamed node" };
	glm::mat4 mLocalTransform{ glm::mat4(1) };
	glm::mat4 mWorldTransform{ glm::mat4(1) };
	Node* mParent{};
	std::vector<Node*> mChildrens{};

	void updateTransform(glm::mat4 parentMatrix)
	{
		mWorldTransform = parentMatrix * mLocalTransform;

		for (Node* children : mChildrens)
			children->updateTransform(mWorldTransform);
	}

	virtual ~Node() = default;
};
