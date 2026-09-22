#pragma once

#include "Descriptor.h"

#define FRAME_OVERLAP 2

struct Frame
{
	VkDevice mDevice{};
	VkCommandPool commandPool{};
	VkCommandBuffer commandBuffer{};
	VkSemaphore acquireSemaphore{};
	VkSemaphore submitSemaphore{};
	VkFence frameFence{};
	Descriptor mDescriptor{};

	// Initialize
	void initializeCommands(VkDevice device, uint32_t queueFamilyIndex);
	void initializeSyncronization();
	void intializeDescriptors(VkDescriptorSetLayout setLayout);

	void cleanupInitialized();
};
