#pragma once

#include "Texture.h"
#include "Descriptor.h"
#include "Buffer.h"

class Renderer;

enum MaterialType
{
	Opaque,
	Transparent
};

struct Material
{
	Texture mColorTexture{};
	VkDescriptorSet mSet{};
	Buffer mUbo{};
	MaterialConstants mConstants{};
	Descriptor* mDescriptor{};
	MaterialType mType{};

	void initializeDescriptor(Descriptor& descriptor, DescriptorLayout& layout);
	void initializeUbo(Renderer& renderer);

	void cleanupInitialized();

	void updateDescriptor(MaterialConstants& materialConstants);
};
