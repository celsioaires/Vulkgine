#pragma once

#include "Descriptor.h"

struct Pipeline
{
	VkDevice mDevice{};
	DescriptorLayout mDescriptorLayout{};
	DescriptorLayout mMaterialDescriptorLayout{};
	Descriptor mDescriptor{};
	Descriptor mMaterialDescriptor{};
	VkDescriptorSet mDescriptorSet{};
	VkShaderModule mComputeShader{};
	VkShaderModule mVertexShader{};
	VkShaderModule mFragmentShader{};
	VkPipeline mComputePipeline{};
	VkPipelineLayout mComputePipelineLayout{};
	VkPipeline mGraphicsPipeline{};
	VkPipelineLayout mGraphicsPipelineLayout{};
	VkSampler mNearestSampler{};
	VkSampler mLinearSampler{};

	// Initialize
	void initializeDescriptors(VkDevice device, VkImageView imageView);
	void initializeShaders();
	void initializeCompute();
	void initializeGraphics(VkDescriptorSetLayout uboDescriptorSetLayout);
	void initializeSampler();

	void cleanupInitialized();

	void updateDescriptors(VkImageView imageView);
};
