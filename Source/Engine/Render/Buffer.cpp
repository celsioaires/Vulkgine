#include "Buffer.h"

#include <vk_mem_alloc.h>

#include "Util.h"

void Buffer::initialize(VkDevice device, VmaAllocator allocator, VkBufferUsageFlags usage, VkDeviceSize size, VmaMemoryUsage memoryUsage)
{
	mAllocator = allocator;
	mSize = size;

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.usage = usage;
	bufferInfo.size = size;
	
	VmaAllocationCreateInfo allocationInfo{};
	allocationInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
	allocationInfo.usage = memoryUsage;

	VK_ASSERT(vmaCreateBuffer(allocator, &bufferInfo, &allocationInfo, &mBuffer, &mAllocation, NULL));
	
	if (usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
	{
		VkBufferDeviceAddressInfo addressInfo{};
		addressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
		addressInfo.buffer = mBuffer;

		mAddress = vkGetBufferDeviceAddress(device, &addressInfo);
		assert(mAddress);
	}
}

void Buffer::cleanup()
{
	vmaDestroyBuffer(mAllocator, mBuffer, mAllocation);
	mBuffer = NULL;
	mAllocation = NULL;

	mAllocator = NULL;
}
