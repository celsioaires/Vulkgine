#pragma once

#include "Texture.h"
#include "Descriptor.h"
#include "Buffer.h"

class Renderer;

struct Material
{
	Texture mColorTexture{};
	VkDescriptorSet mSet{};
	Buffer mUbo{};
	MaterialConstants mConstants{};
	Descriptor* mDescriptor{};

	void initializeDescriptor(Descriptor& descriptor, DescriptorLayout& layout);
	void initializeUbo(Renderer& renderer);

	void cleanupInitialized();

	void updateDescriptor(MaterialConstants& materialConstants);
};
