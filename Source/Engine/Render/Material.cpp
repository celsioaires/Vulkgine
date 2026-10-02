#include "Material.h"

void Material::initializeDescriptor(Descriptor& descriptor, DescriptorLayout& layout)
{
	mSet = descriptor.initializeSet(layout.mHandle);

	descriptor.updateSet(mSet, layout.mType, mColorTexture.mImage.view, mColorTexture.mSampler);
}
