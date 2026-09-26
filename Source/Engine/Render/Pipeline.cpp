#include "Pipeline.h"

#include <vulkan/vulkan.h>

#include "../Core/File.h"
#include "Util.h"

void Pipeline::initializeDescriptors(VkDevice device, VkImageView imageView)
{
	mDevice = device;

	std::vector<DescriptorPoolSize> poolSizes
	{
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1 }
	};

	// Compute
	mDescriptorLayout.initialize(device, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, VK_SHADER_STAGE_COMPUTE_BIT, 2);
	mDescriptor.initializePool(device, poolSizes);
	mDescriptorSet = mDescriptor.initializeSet(mDescriptorLayout.mHandle);
	mDescriptor.updateSet(mDescriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, imageView, 0);
}

void Pipeline::initializeShaders()
{
	std::string exeDirectory = File::getExeDirectory().string();
	std::string computeDirectory = exeDirectory + "\\Shaders\\Compute\\";
	std::string graphicsDirectory = exeDirectory + "\\Shaders\\Graphics\\";

	// Compute
	mComputeShader = Util::createShader(mDevice, (computeDirectory + "Grid.comp.spv").c_str());

	// Graphics
	mVertexShader = Util::createShader(mDevice, (graphicsDirectory + "Default.vert.spv").c_str());
	mFragmentShader = Util::createShader(mDevice, (graphicsDirectory + "Default.frag.spv").c_str());
}

void Pipeline::initializeCompute()
{
	// Push constant
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	pushConstantRange.size = sizeof(ComputePushConstants);

	// Layout
	VkPipelineLayoutCreateInfo computePipelineLayoutInfo{};
	computePipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	computePipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	computePipelineLayoutInfo.pSetLayouts = &mDescriptorLayout.mHandle;
	computePipelineLayoutInfo.pushConstantRangeCount = 1;
	computePipelineLayoutInfo.setLayoutCount = 1;

	// Stage
	VkPipelineShaderStageCreateInfo shaderStageInfo{};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	shaderStageInfo.module = mComputeShader;
	shaderStageInfo.pName = "main";

	// Create layout
	VK_ASSERT(vkCreatePipelineLayout(mDevice, &computePipelineLayoutInfo, 0, &mComputePipelineLayout));

	// Pipeline
	VkComputePipelineCreateInfo computePipelineInfo{};
	computePipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	computePipelineInfo.layout = mComputePipelineLayout;
	computePipelineInfo.stage = shaderStageInfo;

	// Create pipeline
	VK_ASSERT(vkCreateComputePipelines(mDevice, 0, 1, &computePipelineInfo, 0, &mComputePipeline));
}

void Pipeline::initializeGraphics(std::vector<VkDescriptorSetLayout> descriptorSetLayouts)
{
	// Stages
	VkPipelineShaderStageCreateInfo stages[2]{};

	VkPipelineShaderStageCreateInfo stage{};
	stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stage.pName = "main";

	stage.stage = VK_SHADER_STAGE_VERTEX_BIT; // vertex
	stage.module = mVertexShader;
	stages[0] = stage;

	stage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;	// fragment
	stage.module = mFragmentShader;
	stages[1] = stage;

	// Viewport
	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;
	
	// Blending
	VkColorComponentFlags colorComponents =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.colorWriteMask = colorComponents;
	colorBlendAttachment.blendEnable = true;

	VkPipelineColorBlendStateCreateInfo colorBlendState{};
	colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendState.logicOp = VK_LOGIC_OP_COPY;
	colorBlendState.pAttachments = &colorBlendAttachment;
	colorBlendState.attachmentCount = 1;

	// Vertex input
	VkPipelineVertexInputStateCreateInfo vertexInputState{};
	vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	// Input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{};
	inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	// Rasterization
	VkPipelineRasterizationStateCreateInfo rasterizationState{};
	rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationState.lineWidth = 1.0f;

	// Multisample
	VkPipelineMultisampleStateCreateInfo multisampleState{};
	multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleState.minSampleShading = 1.0f;

	// Depth stencil
	VkPipelineDepthStencilStateCreateInfo depthStencilState{};
	depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	depthStencilState.depthWriteEnable = true;
	depthStencilState.depthTestEnable = true;
	depthStencilState.maxDepthBounds = 1.0f;

	// Dynamic
	VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.pDynamicStates = dynamicStates;
	dynamicState.dynamicStateCount = 2;

	// Push constant
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConstantRange.size = sizeof(GraphicsPushConstants);

	// Layout
	VkPipelineLayoutCreateInfo graphicsPipelineLayoutInfo{};
	graphicsPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	graphicsPipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	graphicsPipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	graphicsPipelineLayoutInfo.setLayoutCount = (uint32_t)descriptorSetLayouts.size();
	graphicsPipelineLayoutInfo.pushConstantRangeCount = 1;

	// Rendering 
	VkFormat colorAttachmentFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
	
	VkPipelineRenderingCreateInfo pipelineRenderingInfo{};
	pipelineRenderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	pipelineRenderingInfo.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;
	pipelineRenderingInfo.pColorAttachmentFormats = &colorAttachmentFormat;
	pipelineRenderingInfo.colorAttachmentCount = 1;

	// Create layout
	VK_ASSERT(vkCreatePipelineLayout(mDevice, &graphicsPipelineLayoutInfo, 0, &mGraphicsPipelineLayout));

	// Pipeline
	VkGraphicsPipelineCreateInfo graphicsPipelineInfo{};
	graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineInfo.pNext = &pipelineRenderingInfo;
	graphicsPipelineInfo.pStages = stages;
	graphicsPipelineInfo.pVertexInputState = &vertexInputState;
	graphicsPipelineInfo.pInputAssemblyState = &inputAssemblyState;
	graphicsPipelineInfo.pViewportState = &viewportState;
	graphicsPipelineInfo.pRasterizationState = &rasterizationState;
	graphicsPipelineInfo.pMultisampleState = &multisampleState;
	graphicsPipelineInfo.pColorBlendState = &colorBlendState;
	graphicsPipelineInfo.pDepthStencilState = &depthStencilState;
	graphicsPipelineInfo.pDynamicState = &dynamicState;
	graphicsPipelineInfo.layout = mGraphicsPipelineLayout;
	graphicsPipelineInfo.stageCount = 2;

	// Create pipeline
	VK_ASSERT(vkCreateGraphicsPipelines(mDevice, 0, 1, &graphicsPipelineInfo, 0, &mGraphicsPipeline));
	
	/* PFN_vkSetDebugUtilsObjectNameEXT pfnSetDebugUtilsObjectNameEXT =
		(PFN_vkSetDebugUtilsObjectNameEXT)vkGetDeviceProcAddr(mDevice, "vkSetDebugUtilsObjectNameEXT");

	VkDebugUtilsObjectNameInfoEXT nameInfo{};
	nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	nameInfo.objectType = VK_OBJECT_TYPE_PIPELINE;
	nameInfo.objectHandle = (uint64_t)mGraphicsPipeline;
	nameInfo.pObjectName = "Graphics pipeline";

	VK_ASSERT(pfnSetDebugUtilsObjectNameEXT(mDevice, &nameInfo)); */
}

void Pipeline::initializeSampler()
{
	VkSamplerCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	createInfo.magFilter = VK_FILTER_NEAREST;
	createInfo.minFilter = VK_FILTER_NEAREST;

	VK_ASSERT(vkCreateSampler(mDevice, &createInfo, 0, &mSampler));
}

void Pipeline::cleanupInitialized()
{
	vkDestroySampler(mDevice, mSampler, 0);
	vkDestroyPipelineLayout(mDevice, mGraphicsPipelineLayout, 0);
	vkDestroyPipeline(mDevice, mGraphicsPipeline, 0);
	vkDestroyPipelineLayout(mDevice, mComputePipelineLayout, 0);
	vkDestroyPipeline(mDevice, mComputePipeline, 0);
	vkDestroyShaderModule(mDevice, mFragmentShader, 0);
	vkDestroyShaderModule(mDevice, mVertexShader, 0);
	vkDestroyShaderModule(mDevice, mComputeShader, 0);

	mDescriptor.cleanupInitialized();
	mDescriptorLayout.cleanup();

	mGraphicsPipelineLayout = 0;
	mGraphicsPipeline = 0;
	mComputePipelineLayout = 0;
	mComputePipeline = 0;
	mFragmentShader = 0;
	mVertexShader = 0;
	mComputeShader = 0;

	mDevice = 0;
}

void Pipeline::updateDescriptors(VkImageView imageView)
{
	mDescriptor.updateSet(mDescriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, imageView, 0);
}
