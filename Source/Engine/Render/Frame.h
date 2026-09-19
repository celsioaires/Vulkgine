#pragma once

#include "Fwd.h"

#define FRAME_OVERLAP 2

struct Frame
{
	VkCommandPool commandPool = NULL;
	VkCommandBuffer commandBuffer = NULL;
	VkSemaphore acquireSemaphore = NULL;
	VkSemaphore submitSemaphore = NULL;
	VkFence frameFence = NULL;
};
