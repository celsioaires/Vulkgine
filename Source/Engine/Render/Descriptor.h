#pragma once

#include "Fwd.h"

struct DescriptorLayout
{
	VkDevice mDevice{};
	VkDescriptorSetLayout mHandle{};
	VkDescriptorType mType{};
	uint32_t mBinding{};

	void initialize(VkDevice device, VkDescriptorType type, VkShaderStageFlags stage, uint32_t binding);
	void cleanup();
};

struct Descriptor
{
	VkDevice mDevice{};
	VkDescriptorPool mPool{};
	std::vector<VkDescriptorSet> mSets{};

	void initializePool(VkDevice device, std::vector<DescriptorPoolSize> sizes);
	VkDescriptorSet initializeSet(VkDescriptorSetLayout layout);

	void cleanupInitialized();

	void updateSet(VkDescriptorSet set, VkDescriptorType type, VkImageView imageView, VkSampler sampler);
	void updateSet(VkDescriptorSet set, VkDescriptorType type, VkBuffer buffer);
};
