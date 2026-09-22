#include "Descriptor.h"

#include "Util.h"

void Descriptor::initializePool(VkDevice device)
{
	mDevice = device;

	// Size
	VkDescriptorPoolSize size{};
	size.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	size.descriptorCount = 1;

	// Info
	VkDescriptorPoolCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.pPoolSizes = &size;
	createInfo.poolSizeCount = 1;
	createInfo.maxSets = 1;

	// Create
	VK_ASSERT(vkCreateDescriptorPool(device, &createInfo, 0, &mPool));
}

void Descriptor::initializeLayout()
{
	// Binding
	VkDescriptorSetLayoutBinding layoutBinding{};
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	layoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	layoutBinding.descriptorCount = 1;
	layoutBinding.binding = 0;

	// Info
	VkDescriptorSetLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	createInfo.pBindings = &layoutBinding;
	createInfo.bindingCount = 1;

	// Create
	VK_ASSERT(vkCreateDescriptorSetLayout(mDevice, &createInfo, 0, &mLayout));
}

void Descriptor::initializeSet()
{
	// Info
	VkDescriptorSetAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocateInfo.pSetLayouts = &mLayout;
	allocateInfo.descriptorPool = mPool;
	allocateInfo.descriptorSetCount = 1;

	// Allocate
	VK_ASSERT(vkAllocateDescriptorSets(mDevice, &allocateInfo, &mSet));
}

void Descriptor::cleanupInitialized()
{
	vkDestroyDescriptorSetLayout(mDevice, mLayout, 0);
	vkDestroyDescriptorPool(mDevice, mPool, 0);
}

void Descriptor::updateSet(VkImageView imageView)
{
	// Image
	VkDescriptorImageInfo imageInfo{};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
	imageInfo.imageView = imageView;

	// Write
	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	write.pImageInfo = &imageInfo;
	write.dstSet = mSet;
	write.descriptorCount = 1;
	write.dstBinding = 0;

	// Update
	vkUpdateDescriptorSets(mDevice, 1, &write, 0, 0);
}
