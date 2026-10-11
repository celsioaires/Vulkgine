#include "Material.h"

#include <vulkan/vulkan.h>

void Material::initializeDescriptor(Descriptor& descriptor, DescriptorLayout& layout)
{
	mSet = descriptor.initializeSet(layout.mHandle);

	descriptor.updateSet(mSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, mColorTexture.mImage.view, mColorTexture.mSampler, 1);
}
