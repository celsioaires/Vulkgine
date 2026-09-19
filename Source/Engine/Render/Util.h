#pragma once

#include <fmt/core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "Image.h"

// Vulkan assertion
#define VK_ASSERT(result) \
    do { \
        VkResult r = result; \
        if (r != VK_SUCCESS) { \
			fmt::println("[VK_ASSERT]: {} failed with {}", #result, string_VkResult(r)); \
			assert(false); \
		} \
	} while (0)

class Util
{
public:
	// Cmd
	static void cmdTransitionImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
	static void cmdCopyImageToImage(VkCommandBuffer commandBuffer, VkImage source, VkImage destination, Extent2D srcExtent, Extent2D dstExtent);

	// Helpers
	static VkImageSubresourceRange imageSubresourceRange(VkImageAspectFlags aspectMask);
	static VkSemaphoreSubmitInfo semaphoreSubmitInfo(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore);
	static VkShaderModule createShader(VkDevice device, const char* path);
};
