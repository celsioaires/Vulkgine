#pragma once

#include "Fwd.h"

struct Descriptor
{
	VkDevice mDevice{};
	VkDescriptorPool mPool{};
	VkDescriptorSetLayout mLayout{};
	VkDescriptorSet mSet{};

	void initializePool(VkDevice device);
	void initializeLayout();
	void initializeSet();

	void cleanupInitialized();

	void updateSet(VkImageView imageView);
};
