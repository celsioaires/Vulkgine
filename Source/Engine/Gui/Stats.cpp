#include "Stats.h"

#include <imgui.h>

void Stats::drawPanel(Info info)
{
	ImGui::Begin("Stats");

	ImGui::Text("Triangle count: %i", info.mTriangleCount);

	ImGui::End();
}
