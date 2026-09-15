#include "Pipeline.h"

#include <vulkan/vulkan.h>

#include "../File.h"
#include "Util.h"

void Pipeline::initializeDescriptors(VkDevice device, VkImageView imageView)
{
	mDevice = device;

	// Descriptor set layout
	VkDescriptorSetLayoutBinding setLayoutBinding{};
	setLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	setLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	setLayoutBinding.descriptorCount = 1;
	setLayoutBinding.binding = 0;

	VkDescriptorSetLayoutCreateInfo setLayoutInfo{};
	setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setLayoutInfo.pBindings = &setLayoutBinding;
	setLayoutInfo.bindingCount = 1;

	VK_ASSERT(vkCreateDescriptorSetLayout(device, &setLayoutInfo, NULL, &mDescriptorSetLayout));

	// Descriptor pool
	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	poolSize.descriptorCount = 1;

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.poolSizeCount = 1;
	poolInfo.maxSets = 1;

	VK_ASSERT(vkCreateDescriptorPool(device, &poolInfo, NULL, &mDescriptorPool));

	// Descriptor set
	VkDescriptorSetAllocateInfo setInfo{};
	setInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	setInfo.pSetLayouts = &mDescriptorSetLayout;
	setInfo.descriptorPool = mDescriptorPool;
	setInfo.descriptorSetCount = 1;

	VK_ASSERT(vkAllocateDescriptorSets(device, &setInfo, &mDescriptorSet));

	// Descriptor set write
	VkDescriptorImageInfo descriptorImageInfo{};
	descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
	descriptorImageInfo.imageView = imageView;

	VkWriteDescriptorSet descriptorSetWrite{};
	descriptorSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorSetWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	descriptorSetWrite.pImageInfo = &descriptorImageInfo;
	descriptorSetWrite.dstSet = mDescriptorSet;
	descriptorSetWrite.descriptorCount = 1;
	descriptorSetWrite.dstBinding = 0;

	vkUpdateDescriptorSets(device, 1, &descriptorSetWrite, 0, NULL);
}

void Pipeline::initializeShaders()
{
	std::string exeDirectory = File::getExeDirectory().string();

	// Compute
	std::string computeDirectory = exeDirectory + "\\Shaders\\Compute\\";

	const char* computeFiles[] =
	{
		"Grid.spv"
	};

	for (const char* file : computeFiles)
		mComputeShader = Util::createShader(mDevice, (computeDirectory + file).c_str());

	// Graphics
	std::string graphicsDirectory = exeDirectory + "\\Shaders\\Graphics\\";

	mVertexShader = Util::createShader(mDevice, (graphicsDirectory + "Triangle.vert.spv").c_str());
	mFragmentShader = Util::createShader(mDevice, (graphicsDirectory + "Triangle.frag.spv").c_str());
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
	computePipelineLayoutInfo.pSetLayouts = &mDescriptorSetLayout;
	computePipelineLayoutInfo.pushConstantRangeCount = 1;
	computePipelineLayoutInfo.setLayoutCount = 1;

	VK_ASSERT(vkCreatePipelineLayout(mDevice, &computePipelineLayoutInfo, NULL, &mComputePipelineLayout));

	// Stage
	VkPipelineShaderStageCreateInfo shaderStageInfo{};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	shaderStageInfo.module = mComputeShader;
	shaderStageInfo.pName = "main";

	// Pipeline
	VkComputePipelineCreateInfo computePipelineInfo{};
	computePipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	computePipelineInfo.layout = mComputePipelineLayout;
	computePipelineInfo.stage = shaderStageInfo;

	VK_ASSERT(vkCreateComputePipelines(mDevice, NULL, 1, &computePipelineInfo, NULL, &mComputePipeline));
}

void Pipeline::initializeGraphics(VkInstance instance)
{
	// Stages
	VkPipelineShaderStageCreateInfo stages[2]{};

	VkPipelineShaderStageCreateInfo stage{};
	stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stage.pName = "main";

	// vertex
	stage.stage = VK_SHADER_STAGE_VERTEX_BIT;
	stage.module = mVertexShader;

	stages[0] = stage;

	// fragment
	stage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	stage.module = mFragmentShader;

	stages[1] = stage;

	// States:

	// viewport
	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;
	
	// blending
	VkColorComponentFlags colorComponents =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = colorComponents;

	VkPipelineColorBlendStateCreateInfo colorBlendState{};
	colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendState.logicOp = VK_LOGIC_OP_COPY;
	colorBlendState.pAttachments = &colorBlendAttachment;
	colorBlendState.attachmentCount = 1;

	// vertex input
	VkPipelineVertexInputStateCreateInfo vertexInputState{};
	vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	// input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{};
	inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	// rasterization
	VkPipelineRasterizationStateCreateInfo rasterizationState{};
	rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationState.cullMode = VK_CULL_MODE_NONE;
	rasterizationState.lineWidth = 1.0f;

	// multisample
	VkPipelineMultisampleStateCreateInfo multisampleState{};
	multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleState.minSampleShading = 1.0f;

	// depth stencil
	VkPipelineDepthStencilStateCreateInfo depthStencilState{};
	depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	depthStencilState.depthWriteEnable = true;
	depthStencilState.depthTestEnable = true;
	depthStencilState.maxDepthBounds = 1.0f;

	// dynamic
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
	graphicsPipelineLayoutInfo.pushConstantRangeCount = 1;

	VK_ASSERT(vkCreatePipelineLayout(mDevice, &graphicsPipelineLayoutInfo, NULL, &mGraphicsPipelineLayout));

	// Rendering 
	VkFormat colorAttachmentFormat = VK_FORMAT_R16G16B16A16_SFLOAT;

	VkPipelineRenderingCreateInfo pipelineRenderingInfo{};
	pipelineRenderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	pipelineRenderingInfo.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;
	pipelineRenderingInfo.pColorAttachmentFormats = &colorAttachmentFormat;
	pipelineRenderingInfo.colorAttachmentCount = 1;

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

	VK_ASSERT(vkCreateGraphicsPipelines(mDevice, 0, 1, &graphicsPipelineInfo, NULL, &mGraphicsPipeline));
	
	PFN_vkSetDebugUtilsObjectNameEXT pfnSetDebugUtilsObjectNameEXT =
		(PFN_vkSetDebugUtilsObjectNameEXT)vkGetDeviceProcAddr(mDevice, "vkSetDebugUtilsObjectNameEXT");

	VkDebugUtilsObjectNameInfoEXT nameInfo{};
	nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	nameInfo.objectType = VK_OBJECT_TYPE_PIPELINE;
	nameInfo.objectHandle = (uint64_t)mGraphicsPipeline;
	nameInfo.pObjectName = "Graphics pipeline";

	VK_ASSERT(pfnSetDebugUtilsObjectNameEXT(mDevice, &nameInfo));
}

void Pipeline::cleanupInitialized()
{
	// Compute pipeline layout
	vkDestroyPipelineLayout(mDevice, mGraphicsPipelineLayout, NULL);
	mGraphicsPipelineLayout = NULL;

	// Compute pipeline
	vkDestroyPipeline(mDevice, mGraphicsPipeline, NULL);
	mGraphicsPipeline = NULL;

	// Compute pipeline layout
	vkDestroyPipelineLayout(mDevice, mComputePipelineLayout, NULL);
	mComputePipelineLayout = NULL;

	// Compute pipeline
	vkDestroyPipeline(mDevice, mComputePipeline, NULL);
	mComputePipeline = NULL;

	// Graphics shaders
	vkDestroyShaderModule(mDevice, mFragmentShader, NULL);
	vkDestroyShaderModule(mDevice, mVertexShader, NULL);

	mFragmentShader = NULL;
	mVertexShader = NULL;

	// Compute shaders
	vkDestroyShaderModule(mDevice, mComputeShader, NULL);
	mComputeShader = NULL;

	// Descriptor pool
	vkDestroyDescriptorPool(mDevice, mDescriptorPool, NULL);
	mDescriptorPool = NULL;

	// Descriptor set layout
	vkDestroyDescriptorSetLayout(mDevice, mDescriptorSetLayout, NULL);
	mDescriptorSetLayout = NULL;

	mDevice = NULL;
}
