#include "Material.h"

#include <vma/vk_mem_alloc.h>

#include "Renderer.h"

void Material::initializeDescriptor(Descriptor& descriptor, DescriptorLayout& layout)
{
	mSet = descriptor.initializeSet(layout.mHandle);
	mDescriptor = &descriptor;
}

void Material::initializeUbo(Renderer& renderer)
{
	Context& renderingContext = renderer.getContext();

	mUbo.initialize(renderingContext.mDevice, renderingContext.allocator, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, sizeof(mConstants), VMA_MEMORY_USAGE_CPU_TO_GPU);
}

void Material::cleanupInitialized()
{
	mUbo.cleanup();
}

void Material::updateDescriptor(MaterialConstants& materialConstants)
{
	mUbo.mapData(&materialConstants);

	mDescriptor->updateSet(mSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, mUbo.mBuffer, 0);
	mDescriptor->updateSet(mSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, mColorTexture.mImage.view, mColorTexture.mSampler, 1);
}
