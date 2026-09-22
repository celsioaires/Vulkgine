#include "Camera.h"

#include <vma/vk_mem_alloc.h>

void Camera::initializeUbo(VkDevice device, VmaAllocator allocator)
{
	mUbo.initialize(device, allocator, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, sizeof(mMvp), VMA_MEMORY_USAGE_CPU_TO_GPU);
}

void Camera::cleanupInitialized()
{
	mUbo.cleanup();
}
