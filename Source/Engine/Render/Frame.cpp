#include "Frame.h"

#include "Util.h"

void Frame::initializeCommands(VkDevice device, uint32_t queueFamilyIndex)
{
	mDevice = device;

	// Pool
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndex;

	// Info
	VkCommandBufferAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.commandBufferCount = 1;

	// Create
	VK_ASSERT(vkCreateCommandPool(mDevice, &poolInfo, 0, &commandPool));

	// Allocate
	allocateInfo.commandPool = commandPool;

	VK_ASSERT(vkAllocateCommandBuffers(mDevice, &allocateInfo, &commandBuffer));	
}

void Frame::initializeSyncronization()
{
	// Semaphore
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	// Fence
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	// Create
	VK_ASSERT(vkCreateSemaphore(mDevice, &semaphoreInfo, 0, &acquireSemaphore));
	VK_ASSERT(vkCreateSemaphore(mDevice, &semaphoreInfo, 0, &submitSemaphore));
	VK_ASSERT(vkCreateFence(mDevice, &fenceInfo, 0, &frameFence));
}

void Frame::intializeDescriptors()
{

}

void Frame::cleanupInitialized()
{
	vkDestroySemaphore(mDevice, acquireSemaphore, 0);
	vkDestroySemaphore(mDevice, submitSemaphore, 0);
	vkDestroyFence(mDevice, frameFence, 0);
	vkDestroyCommandPool(mDevice, commandPool, 0);

	acquireSemaphore = 0;
	submitSemaphore = 0;
	frameFence = 0;
	commandPool = 0;
}
