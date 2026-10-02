#pragma once

#include <vector>

#include "../Scene/Node.h"

class Hierarchy
{
	Node* mSelectedNode{};
public:
	void initialize(Node* node);

	void drawPanel(std::vector<Node*>& nodes);

	Node* getSelectedNode() { return mSelectedNode; }
};
