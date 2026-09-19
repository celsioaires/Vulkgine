#pragma once

#include "Fwd.h"

struct Pipeline
{
	VkDevice mDevice = NULL;
	VkDescriptorPool mDescriptorPool = NULL;
	VkDescriptorSetLayout mDescriptorSetLayout = NULL;
	VkDescriptorSet mDescriptorSet = NULL;
	VkShaderModule mComputeShader = NULL;
	VkShaderModule mVertexShader = NULL;
	VkShaderModule mFragmentShader = NULL;
	VkPipeline mComputePipeline = NULL;
	VkPipelineLayout mComputePipelineLayout = NULL;
	VkPipeline mGraphicsPipeline = NULL;
	VkPipelineLayout mGraphicsPipelineLayout = NULL;

	// Initialize
	void initializeDescriptors(VkDevice device, VkImageView imageView);
	void initializeShaders();
	void initializeCompute();
	void initializeGraphics();

	void cleanupInitialized();

	void updateDescriptors(VkImageView imageView);
};
