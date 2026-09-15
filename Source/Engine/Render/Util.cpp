#include "Util.h"

#include <vector>
#include <fstream>

#include <fmt/core.h>
#include <vulkan/vulkan.h>

void Util::cmdTransitionImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkImageAspectFlags aspectMask;

	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL)
		aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	else
		aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	VkImageMemoryBarrier2 imageBarrier{};
	imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
	imageBarrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
	imageBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
	imageBarrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
	imageBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;
	imageBarrier.subresourceRange = imageSubresourceRange(aspectMask);
	imageBarrier.oldLayout = oldLayout;
	imageBarrier.newLayout = newLayout;
	imageBarrier.image = image;

	VkDependencyInfo dependencyInfo{};
	dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
	dependencyInfo.pImageMemoryBarriers = &imageBarrier;
	dependencyInfo.imageMemoryBarrierCount = 1;

	vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
}

void Util::cmdCopyImageToImage(VkCommandBuffer commandBuffer, VkImage source, VkImage destination, Extent2D srcExtent, Extent2D dstExtent)
{
	VkImageSubresourceLayers subresourceLayer{};
	subresourceLayer.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresourceLayer.layerCount = 1;

	VkImageBlit2 blitRegion{};
	blitRegion.sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2;
	blitRegion.srcSubresource = subresourceLayer;
	blitRegion.dstSubresource = subresourceLayer;
	blitRegion.srcOffsets[1].x = srcExtent.width;
	blitRegion.srcOffsets[1].y = srcExtent.height;
	blitRegion.dstOffsets[1].x = dstExtent.width;
	blitRegion.dstOffsets[1].y = dstExtent.height;
	blitRegion.srcOffsets[1].z = 1;
	blitRegion.dstOffsets[1].z = 1;

	VkBlitImageInfo2 blitInfo{};
	blitInfo.sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2;
	blitInfo.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	blitInfo.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	blitInfo.filter = VK_FILTER_LINEAR;
	blitInfo.srcImage = source;
	blitInfo.dstImage = destination;
	blitInfo.pRegions = &blitRegion;
	blitInfo.regionCount = 1;

	vkCmdBlitImage2(commandBuffer, &blitInfo);
}

VkImageSubresourceRange Util::imageSubresourceRange(VkImageAspectFlags aspectMask)
{
	VkImageSubresourceRange subresourceRange{};
	subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
	subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
	subresourceRange.aspectMask = aspectMask;

	return subresourceRange;
}

VkSemaphoreSubmitInfo Util::semaphoreSubmitInfo(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore)
{
	VkSemaphoreSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
	submitInfo.semaphore = semaphore;
	submitInfo.stageMask = stageMask;
	submitInfo.value = 1;

	return submitInfo;
}

VkShaderModule Util::createShader(VkDevice device, const char* path)
{
	VkShaderModule module = NULL;

	std::ifstream file(path, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		fmt::println("Failed to open shader '{}'", path);
		return module;
	}

	size_t size = file.tellg();
	std::vector<uint32_t> content(size / sizeof(uint32_t));

	file.seekg(0);
	file.read((char*)content.data(), size);
	file.close();

	VkShaderModuleCreateInfo moduleInfo{};
	moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	moduleInfo.pCode = content.data();
	moduleInfo.codeSize = size;

	VK_ASSERT(vkCreateShaderModule(device, &moduleInfo, NULL, &module));

	return module;
}
