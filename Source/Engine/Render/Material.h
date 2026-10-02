#pragma once

#include "Texture.h"
#include "Descriptor.h"

struct Material
{
	Texture mColorTexture{};
	VkDescriptorSet mSet{};

	void initializeDescriptor(Descriptor& descriptor, DescriptorLayout& layout);
};
