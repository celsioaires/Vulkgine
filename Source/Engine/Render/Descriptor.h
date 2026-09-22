#pragma once

#include "Fwd.h"

struct DescriptorLayout
{
	VkDevice mDevice{};
	VkDescriptorSetLayout mHandle{};

	void initialize(VkDevice device, VkDescriptorType type, VkShaderStageFlags stage);
	void cleanup();
};

struct Descriptor
{
	VkDevice mDevice{};
	VkDescriptorPool mPool{};
	VkDescriptorSet mSet{};

	void initializePool(VkDevice device, VkDescriptorType type);
	void initializeSet(VkDescriptorSetLayout layout);

	void cleanupInitialized();

	void updateSet(VkImageView imageView);
	void updateSet(VkBuffer buffer);
};
