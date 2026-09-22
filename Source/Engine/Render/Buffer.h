#pragma once

#include "Fwd.h"

struct Buffer
{
	VmaAllocator mAllocator{};
	VkBuffer mBuffer{};
	VmaAllocation mAllocation{};
	VkDeviceSize mSize{};
	VkDeviceAddress mAddress{};

	void initialize(VkDevice device, VmaAllocator allocator, VkBufferUsageFlags usage, VkDeviceSize size, VmaMemoryUsage memoryUsage);
	void cleanup();

	void mapData(void* data);
};
