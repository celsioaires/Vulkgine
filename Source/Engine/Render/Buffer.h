#pragma once

#include "Fwd.h"

struct Buffer
{
	VmaAllocator mAllocator = NULL;
	VkBuffer mBuffer = NULL;
	VmaAllocation mAllocation = NULL;
	VkDeviceSize mSize = 0;
	VkDeviceAddress mAddress = 0;

	void initialize(VkDevice device, VmaAllocator allocator, VkBufferUsageFlags usage, VkDeviceSize size, VmaMemoryUsage memoryUsage);
	void cleanup();
};
