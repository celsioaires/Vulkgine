#pragma once

#include "../Scene/Node.h"
#include "../Render/Fwd.h"

class Inspector
{
public:
	void drawPanel(Node* node, ComputeEffect& computeEffect);
};
