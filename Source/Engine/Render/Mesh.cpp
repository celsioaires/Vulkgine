#include "Mesh.h"

#include <vma/vk_mem_alloc.h>

#include "Renderer.h"
#include "Util.h"

void Mesh::initializeBuffers(Renderer& renderer, std::span<Vertex> vertices, std::span<uint32_t> indices)
{
	Context& renderingContext = renderer.getContext();

	VkDevice device = renderingContext.mDevice;
	VmaAllocator allocator = renderingContext.allocator;

	VkBufferUsageFlags bufferUsages =
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
		VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

	VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY;

	mVertexBuffer.initialize(device, allocator, bufferUsages, vertices.size_bytes(), memoryUsage);

	bufferUsages = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	mIndexBuffer.initialize(device, allocator, bufferUsages, indices.size_bytes(), memoryUsage);

	// Staging buffer
	Buffer stagingBuffer;
	VkDeviceSize stagingBufferSize = mVertexBuffer.mSize + mIndexBuffer.mSize;

	stagingBuffer.initialize(device, allocator, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBufferSize, VMA_MEMORY_USAGE_CPU_ONLY);

	void* bufferData = NULL;
	VK_ASSERT(vmaMapMemory(allocator, stagingBuffer.mAllocation, &bufferData));

	memcpy(bufferData, vertices.data(), mVertexBuffer.mSize);
	memcpy((char*)bufferData + mVertexBuffer.mSize, indices.data(), mIndexBuffer.mSize);

	vmaUnmapMemory(allocator, stagingBuffer.mAllocation);

	// Copy to staging buffer
	VkCommandBuffer commandBuffer = renderer.beginImmediateRender();

	VkBufferCopy bufferCopy{};
	bufferCopy.size = mVertexBuffer.mSize;

	vkCmdCopyBuffer(commandBuffer, stagingBuffer.mBuffer, mVertexBuffer.mBuffer, 1, &bufferCopy);

	bufferCopy.srcOffset = mVertexBuffer.mSize;
	bufferCopy.size = mIndexBuffer.mSize;

	vkCmdCopyBuffer(commandBuffer, stagingBuffer.mBuffer, mIndexBuffer.mBuffer, 1, &bufferCopy);

	renderer.endImmediateRender(commandBuffer);

	stagingBuffer.cleanup();
}

void Mesh::cleanupInitialized()
{
	mIndexBuffer.cleanup();
	mVertexBuffer.cleanup();
}
