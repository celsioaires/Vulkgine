#pragma once

#include "Fwd.h"

struct DescriptorLayout
{
	VkDevice mDevice{};
	VkDescriptorSetLayout mHandle{};

	void initialize(VkDevice device, std::vector<DescriptorSetBinding>& setBindings);
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

	void updateSet(VkDescriptorSet set, VkDescriptorType type, VkImageView imageView, VkSampler sampler, uint32_t binding);
	void updateSet(VkDescriptorSet set, VkDescriptorType type, VkBuffer buffer, uint32_t binding);
};
