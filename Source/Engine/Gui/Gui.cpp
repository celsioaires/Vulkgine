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

	ImGui::StyleColorsLight();

	assert(ImGui_ImplSDL2_InitForVulkan(window));

	VkFormat colorAttachmentFormat = VK_FORMAT_R16G16B16A16_SFLOAT;

	VkPipelineRenderingCreateInfoKHR pipelineRenderingInfo{};
	pipelineRenderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	pipelineRenderingInfo.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT; // TODO: remove
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

void Gui::drawPanels(ComputeEffect& computeEffect)
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame();

	ImGui::NewFrame();

	mInspector.drawPanel(computeEffect);

	ImGui::Render();
}

void Gui::renderPanels(VkCommandBuffer commandBuffer)
{
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
}
