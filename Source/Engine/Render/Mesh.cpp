#include "Mesh.h"

#include <vma/vk_mem_alloc.h>

#include "Renderer.h"
#include "Util.h"

void Mesh::initializeBuffers(Renderer& renderer, std::span<Vertex> vertices, std::span<uint32_t> indices)
{
	Context& context = renderer.getContext();

	VkDevice device = context.mDevice;
	VmaAllocator allocator = context.allocator;

	// Vertex
	VkBufferUsageFlags bufferUsages =
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
		VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

	VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY;

	mVertexBuffer.initialize(device, allocator, bufferUsages, vertices.size_bytes(), memoryUsage);

	// Index
	bufferUsages = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	mIndexBuffer.initialize(device, allocator, bufferUsages, indices.size_bytes(), memoryUsage);

	// Staging
	Buffer buffer{};
	VkDeviceSize bufferSize = mVertexBuffer.mSize + mIndexBuffer.mSize;

	buffer.initialize(device, allocator, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, bufferSize, VMA_MEMORY_USAGE_CPU_ONLY);
	
	// Mapping
	void* bufferData{};

	VK_ASSERT(vmaMapMemory(allocator, buffer.mAllocation, &bufferData));

	memcpy(bufferData, vertices.data(), mVertexBuffer.mSize);
	memcpy((char*)bufferData + mVertexBuffer.mSize, indices.data(), mIndexBuffer.mSize);

	vmaUnmapMemory(allocator, buffer.mAllocation);

	// Copy
	VkCommandBuffer commandBuffer = renderer.beginImmediateRender();

	VkBufferCopy bufferCopy{};
	bufferCopy.size = mVertexBuffer.mSize;

	vkCmdCopyBuffer(commandBuffer, buffer.mBuffer, mVertexBuffer.mBuffer, 1, &bufferCopy);

	bufferCopy.srcOffset = mVertexBuffer.mSize;
	bufferCopy.size = mIndexBuffer.mSize;

	vkCmdCopyBuffer(commandBuffer, buffer.mBuffer, mIndexBuffer.mBuffer, 1, &bufferCopy);

	renderer.endImmediateRender(commandBuffer);

	// Cleanup
	buffer.cleanup();
}

void Mesh::cleanupInitialized()
{
	mIndexBuffer.cleanup();
	mVertexBuffer.cleanup();
}
