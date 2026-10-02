#include "Hierarchy.h"

#include <imgui.h>

void Hierarchy::initialize(Node* node)
{
    mSelectedNode = node;
}

void Hierarchy::drawPanel(std::vector<Node*>& nodes)
{
	ImGui::Begin("Hierarchy");

    for (Node* node : nodes)
    {
        bool selected = (mSelectedNode == node);

        if (ImGui::Selectable(node->mName.c_str(), selected))
            mSelectedNode = node;
    }

	ImGui::End();
}
