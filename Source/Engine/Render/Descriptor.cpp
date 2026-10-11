#include "Descriptor.h"

#include "Util.h"

void DescriptorLayout::initialize(VkDevice device, std::vector<DescriptorSetBinding>& setBindings)
{
	mDevice = device;

	std::vector<VkDescriptorSetLayoutBinding> vkSetBindings{};

	for (DescriptorSetBinding& setBinding : setBindings)
	{
		VkDescriptorSetLayoutBinding vkSetBinding{};
		vkSetBinding.binding = setBinding.mBinding;
		vkSetBinding.descriptorType = setBinding.mType;
		vkSetBinding.stageFlags = setBinding.mStage;
		vkSetBinding.descriptorCount = 1;

		vkSetBindings.push_back(vkSetBinding);
	}

	// Info
	VkDescriptorSetLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	createInfo.pBindings = vkSetBindings.data();
	createInfo.bindingCount = (uint32_t)vkSetBindings.size();

	// Create
	VK_ASSERT(vkCreateDescriptorSetLayout(mDevice, &createInfo, 0, &mHandle));
}

void DescriptorLayout::cleanup()
{
	vkDestroyDescriptorSetLayout(mDevice, mHandle, 0);
	mHandle = 0;
}

void Descriptor::initializePool(VkDevice device, std::vector<DescriptorPoolSize> sizes)
{
	mDevice = device;

	std::vector<VkDescriptorPoolSize> vkSizes(sizes.size());

	int i{};

	for (DescriptorPoolSize size : sizes)
	{
		vkSizes[i].type = size.mType;
		vkSizes[i].descriptorCount = size.mDescriptorCount;

		i++;
	}

	// Info
	VkDescriptorPoolCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.pPoolSizes = vkSizes.data();
	createInfo.poolSizeCount = (uint32_t)sizes.size();
	createInfo.maxSets = 10000;

	// Create
	VK_ASSERT(vkCreateDescriptorPool(device, &createInfo, 0, &mPool));
}

VkDescriptorSet Descriptor::initializeSet(VkDescriptorSetLayout layout)
{
	VkDescriptorSet set{};

	// Info
	VkDescriptorSetAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocateInfo.pSetLayouts = &layout;
	allocateInfo.descriptorPool = mPool;
	allocateInfo.descriptorSetCount = 1;

	// Allocate
	VK_ASSERT(vkAllocateDescriptorSets(mDevice, &allocateInfo, &set));

	mSets.push_back(set);

	return set;
}

void Descriptor::cleanupInitialized()
{
	vkDestroyDescriptorPool(mDevice, mPool, 0);
	mPool = 0;
}

void Descriptor::updateSet(VkDescriptorSet set, VkDescriptorType type, VkImageView imageView, VkSampler sampler, uint32_t binding)
{
	// Image
	VkDescriptorImageInfo imageInfo{};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
	imageInfo.imageView = imageView;

	if (sampler)
	{
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.sampler = sampler;
	}

	// Write
	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.pImageInfo = &imageInfo;
	write.descriptorType = type;
	write.dstBinding = binding;
	write.dstSet = set;
	write.descriptorCount = 1;

	// Update
	vkUpdateDescriptorSets(mDevice, 1, &write, 0, 0);
}

void Descriptor::updateSet(VkDescriptorSet set, VkDescriptorType type, VkBuffer buffer)
{
	// Image
	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.range = VK_WHOLE_SIZE;
	bufferInfo.buffer = buffer;

	// Write
	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorType = type;
	write.pBufferInfo = &bufferInfo;
	write.dstSet = set;
	write.descriptorCount = 1;
	write.dstBinding = 0;

	// Update
	vkUpdateDescriptorSets(mDevice, 1, &write, 0, 0);
}
