#pragma once

#include <cassert>

#include "Image.h"

#define VK_ASSERT(result) assert(result == 0)

class Util
{
public:
	// Cmd
	static void cmdTransitionImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
	static void cmdCopyImageToImage(VkCommandBuffer commandBuffer, VkImage source, VkImage destination, Extent2D srcExtent, Extent2D dstExtent);

	static VkImageSubresourceRange imageSubresourceRange(VkImageAspectFlags aspectMask);
	static VkSemaphoreSubmitInfo semaphoreSubmitInfo(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore);
	static VkShaderModule createShader(VkDevice device, const char* path);
};
