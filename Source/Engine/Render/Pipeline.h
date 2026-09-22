#pragma once

#include "Descriptor.h"

struct Pipeline
{
	VkDevice mDevice{};
	/*VkDescriptorPool mDescriptorPool{};
	VkDescriptorSetLayout mDescriptorSetLayout{};
	VkDescriptorSet mDescriptorSet{};*/
	Descriptor mDescriptor{};
	VkShaderModule mComputeShader{};
	VkShaderModule mVertexShader{};
	VkShaderModule mFragmentShader{};
	VkPipeline mComputePipeline{};
	VkPipelineLayout mComputePipelineLayout{};
	VkPipeline mGraphicsPipeline{};
	VkPipelineLayout mGraphicsPipelineLayout{};

	// Initialize
	void initializeDescriptors(VkDevice device, VkImageView imageView);
	void initializeShaders();
	void initializeCompute();
	void initializeGraphics();

	void cleanupInitialized();

	void updateDescriptors(VkImageView imageView);
};
