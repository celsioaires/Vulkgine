#pragma once

#include "Descriptor.h"

struct Pipeline
{
	VkDevice mDevice{};
	DescriptorLayout mDescriptorLayout{};
	Descriptor mDescriptor{};
	VkDescriptorSet mDescriptorSet{};
	VkShaderModule mComputeShader{};
	VkShaderModule mVertexShader{};
	VkShaderModule mFragmentShader{};
	VkPipeline mComputePipeline{};
	VkPipelineLayout mComputePipelineLayout{};
	VkPipeline mGraphicsPipeline{};
	VkPipelineLayout mGraphicsPipelineLayout{};
	VkSampler mSampler{};

	// Initialize
	void initializeDescriptors(VkDevice device, VkImageView imageView);
	void initializeShaders();
	void initializeCompute();
	void initializeGraphics(std::vector<VkDescriptorSetLayout> descriptorSetLayouts);
	void initializeSampler();

	void cleanupInitialized();

	void updateDescriptors(VkImageView imageView);
};
