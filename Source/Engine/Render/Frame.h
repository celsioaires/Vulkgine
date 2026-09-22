#pragma once

#include "Descriptor.h"

#define FRAME_OVERLAP 2

struct Frame
{
	VkCommandPool commandPool{};
	VkCommandBuffer commandBuffer{};
	VkSemaphore acquireSemaphore{};
	VkSemaphore submitSemaphore{};
	VkFence frameFence{};
	Descriptor mDescriptors{};
};
