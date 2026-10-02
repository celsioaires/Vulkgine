#include "Inspector.h"

#include <imgui.h>

void Inspector::drawPanel(Node* node, ComputeEffect& computeEffect)
{
	ImGui::Begin("Inspector");

	if (node)
	{
		ImGui::Text("%s", node->mName.c_str());

		auto position = glm::vec3(node->mTransform[3]);

		if (ImGui::DragFloat3("Position", &position.x, 0.1f))
			node->mTransform[3] = glm::vec4(position, 1.0f);
	}

	ImGui::End();

	ImGui::Begin("Compute effect");

	ImGui::Text("Constants:");

	ComputePushConstants& pushConstants = computeEffect.mPushConstants;
	ImGui::SliderFloat4("data", (float*)&pushConstants.data1, 0.0f, 1.0f);

	/* 
	ImGui::SliderFloat4("data 2", (float*)&pushConstants.data2, 0.0f, 1.0f);
	ImGui::SliderFloat4("data 3", (float*)&pushConstants.data3, 0.0f, 1.0f);
	ImGui::SliderFloat4("data 4", (float*)&pushConstants.data4, 0.0f, 1.0f); */

	ImGui::End();
}
