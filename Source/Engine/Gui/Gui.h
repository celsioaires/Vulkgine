#pragma once

#include "../Render/Fwd.h"

#include "Inspector.h"
#include "Hierarchy.h"

#include "../Scene/Node.h"

class Gui
{
private:
	VkDevice mDevice{};
	VkDescriptorPool mDescriptorPool{};
	Inspector mInspector{};
	Hierarchy mHierarchy{};
public:
	void initializeDescriptors(VkDevice device);
	void initializeContext(SDL_Window* window, VkInstance instance, VkPhysicalDevice physicalDevice, VkQueue queue);

	void cleanupInitialized();

	void updatePanels(SDL_Event* event);
	void drawPanels(std::vector<Node*>& nodes, ComputeEffect& computeEffect, float& renderScale);

	void renderPanels(VkCommandBuffer commandBuffer, VkImageView imageView, Extent2D extent);
};
