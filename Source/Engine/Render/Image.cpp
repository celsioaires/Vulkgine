#include "Image.h"

#include <vma/vk_mem_alloc.h>

#include "Util.h"

void Image::initialize(VmaAllocator allocator, VkFormat format, VkImageUsageFlags usages, uint32_t width, uint32_t height)
{
	mAllocator = allocator;
	mFormat = format;
	extent = { width, height, 1 };

	VkImageCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	createInfo.imageType = VK_IMAGE_TYPE_2D;
	createInfo.extent = { width, height, 1 };
	createInfo.format = format;
	createInfo.usage = usages;
	createInfo.arrayLayers = 1;
	createInfo.mipLevels = 1;

	VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	VmaAllocationCreateInfo allocationInfo{};
	allocationInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	allocationInfo.requiredFlags = memoryFlags;

	VK_ASSERT(vmaCreateImage(allocator, &createInfo, &allocationInfo, &handle, &allocation, 0));
}

void Image::initializeView(VkDevice device, VkImageAspectFlags aspect)
{
	mDevice = device;

	VkImageSubresourceRange subresource{};
	subresource.aspectMask = aspect;
	subresource.levelCount = 1;
	subresource.layerCount = 1;

	VkImageViewCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.subresourceRange = subresource;
	createInfo.image = handle;
	createInfo.format = mFormat;

	VK_ASSERT(vkCreateImageView(mDevice, &createInfo, 0, &view));
}

void Image::cleanupInitialized()
{
	vkDestroyImageView(mDevice, view, 0);
	vmaDestroyImage(mAllocator, handle, allocation);

	view = 0;
	handle = 0;

	allocation = 0;
}
