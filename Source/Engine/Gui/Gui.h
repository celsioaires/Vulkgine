#pragma once

#include "../Render/Fwd.h"

#include "Inspector.h"

class Gui
{
private:
	VkDevice mDevice = NULL;
	VkDescriptorPool mDescriptorPool = NULL;
	Inspector mInspector;
public:
	void initializeDescriptors(VkDevice device);
	void initializeContext(SDL_Window* window, VkInstance instance, VkPhysicalDevice physicalDevice, VkQueue queue);

	void cleanupInitialized();

	void updatePanels(SDL_Event* event);
	void drawPanels(ComputeEffect& computeEffect);

	void renderPanels(VkCommandBuffer commandBuffer);
};
