#include "Inspector.h"

#include <imgui.h>

void Inspector::drawPanel(ComputeEffect& computeEffect)
{
	ComputePushConstants& pushConstants = computeEffect.mPushConstants;

	ImGui::Begin("Inspector");

	ImGui::Text("Compute push constants:");
	ImGui::SliderFloat4("data 1", (float*)&pushConstants.data1, 0.0f, 1.0f);

	/* 
	ImGui::SliderFloat4("data 2", (float*)&pushConstants.data2, 0.0f, 1.0f);
	ImGui::SliderFloat4("data 3", (float*)&pushConstants.data3, 0.0f, 1.0f);
	ImGui::SliderFloat4("data 4", (float*)&pushConstants.data4, 0.0f, 1.0f); */

	ImGui::End();
}
