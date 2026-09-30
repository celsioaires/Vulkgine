#include "Hierarchy.h"

#include <imgui.h>

void Hierarchy::drawPanel(std::vector<Node*>& nodes)
{
	ImGui::Begin("Hierarchy");

    int i{};

    for (Node* node : nodes)
    {
		ImGui::PushID(i++);

        auto position = glm::vec3(node->mTransform[3]);

        if (ImGui::DragFloat3("Position", &position.x, 0.1f))
            node->mTransform[3] = glm::vec4(position, 1.0f);

        ImGui::PopID();
    }

	ImGui::End();
}
