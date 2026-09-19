#include "Gui.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_vulkan.h>

#include "../Render/Util.h"

void Gui::initializeDescriptors(VkDevice device)
{
	mDevice = device;

	VkDescriptorPoolSize descriptorPoolSizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo descriptorPoolInfo{};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descriptorPoolInfo.pPoolSizes = descriptorPoolSizes;
	descriptorPoolInfo.poolSizeCount = IM_COUNTOF(descriptorPoolSizes);
	descriptorPoolInfo.maxSets = 1000;

	VK_ASSERT(vkCreateDescriptorPool(device, &descriptorPoolInfo, NULL, &mDescriptorPool));
}

void Gui::initializeContext(SDL_Window* window, VkInstance instance, VkPhysicalDevice physicalDevice, VkQueue queue)
{
	assert(ImGui::CreateContext());

	//ImGui::StyleColorsLight();
	ImGui::StyleColorsClassic();

	assert(ImGui_ImplSDL2_InitForVulkan(window));

	VkFormat colorAttachmentFormat = VK_FORMAT_B8G8R8A8_UNORM;

	VkPipelineRenderingCreateInfoKHR pipelineRenderingInfo{};
	pipelineRenderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	pipelineRenderingInfo.pColorAttachmentFormats = &colorAttachmentFormat;
	pipelineRenderingInfo.colorAttachmentCount = 1;

	ImGui_ImplVulkan_PipelineInfo pipelineInfo{};
	pipelineInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	pipelineInfo.PipelineRenderingCreateInfo = pipelineRenderingInfo;
	
	ImGui_ImplVulkan_InitInfo initInfo{};
	initInfo.PipelineInfoMain = pipelineInfo;
	initInfo.DescriptorPool = mDescriptorPool;
	initInfo.Device = mDevice;
	initInfo.PhysicalDevice = physicalDevice;
	initInfo.Instance = instance;
	initInfo.Queue = queue;
	initInfo.UseDynamicRendering = true;
	initInfo.MinImageCount = 3;
	initInfo.ImageCount = 3;

	assert(ImGui_ImplVulkan_Init(&initInfo));
}

void Gui::cleanupInitialized()
{
	// Context
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	// Descriptors
	vkDestroyDescriptorPool(mDevice, mDescriptorPool, NULL);
	mDescriptorPool = NULL;

	mDevice = NULL;
}

void Gui::updatePanels(SDL_Event* event)
{
	ImGui_ImplSDL2_ProcessEvent(event);
}

void Gui::drawPanels(ComputeEffect& computeEffect, float& renderScale)
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame();

	ImGui::NewFrame();

	mInspector.drawPanel(computeEffect);

	ImGui::Begin("Dynamic resolution");
	ImGui::SliderFloat("Render scale", &renderScale, 0.3f, 1.f);
	ImGui::End();

	ImGui::Render();
}

void Gui::renderPanels(VkCommandBuffer commandBuffer, VkImageView imageView, Extent2D extent)
{
	// Color attachment
	VkRenderingAttachmentInfo colorAttachmentInfo{};
	colorAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
	colorAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	colorAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentInfo.imageView = imageView;
	colorAttachmentInfo.clearValue.color = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Begin rendering
	VkRenderingInfo renderingInfo{};
	renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
	renderingInfo.renderArea.extent = { extent.width, extent.height };
	renderingInfo.pColorAttachments = &colorAttachmentInfo;
	renderingInfo.colorAttachmentCount = 1;
	renderingInfo.layerCount = 1;

	vkCmdBeginRendering(commandBuffer, &renderingInfo);

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer); // ImGui rendering

	vkCmdEndRendering(commandBuffer);
}
