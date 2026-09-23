#include "Texture.h"

#include <vma/vk_mem_alloc.h>

#include "Renderer.h"
#include "Util.h"

void Texture::initializeImage(Renderer& renderer, void* pixels, uint32_t width, uint32_t height)
{
	Context& context = renderer.getContext();

	VkDevice device = context.mDevice;
	VmaAllocator allocator = context.allocator;

	// Image
	VkImageUsageFlags usages =
		VK_IMAGE_USAGE_TRANSFER_DST_BIT |
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
		VK_IMAGE_USAGE_SAMPLED_BIT;

	mImage.initialize(allocator, VK_FORMAT_R8G8B8A8_UNORM, usages, width, height);
	mImage.initializeView(device, VK_IMAGE_ASPECT_COLOR_BIT);

	// Staging
	Buffer buffer{};
	VkDeviceSize bufferSize = 1.0f * width * height * 4;

	buffer.initialize(device, allocator, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, bufferSize, VMA_MEMORY_USAGE_CPU_TO_GPU);
	
	// Mapping
	void* bufferData{};

	VK_ASSERT(vmaMapMemory(allocator, buffer.mAllocation, &bufferData));

	memcpy(bufferData, pixels, bufferSize);

	vmaUnmapMemory(allocator, buffer.mAllocation);

	// Copy
	VkCommandBuffer commandBuffer = renderer.beginImmediateRender();
	VkImage image = mImage.handle;

	Util::cmdTransitionImage(commandBuffer, image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	
	VkImageSubresourceLayers subresourceLayers{};
	subresourceLayers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresourceLayers.layerCount = 1;

	VkBufferImageCopy bufferCopy{};
	bufferCopy.imageSubresource = subresourceLayers;
	bufferCopy.imageExtent = { width, height, 1 };

	vkCmdCopyBufferToImage(commandBuffer, buffer.mBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopy);

	Util::cmdTransitionImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	
	renderer.endImmediateRender(commandBuffer);

	// Staging
	buffer.cleanup();
}

void Texture::cleanupInitialized()
{
	mImage.cleanupInitialized();
}
