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

	ImGui::StyleColorsClassic();

	ImVec4 yellow = ImVec4(1.00f, 0.75f, 0.00f, 1.00f);
	ImVec4 yellowHover = ImVec4(1.00f, 0.85f, 0.05f, 1.00f);
	ImVec4 yellowActive = ImVec4(1.00f, 0.95f, 0.15f, 1.00f);
	ImVec4 dark = ImVec4(0.08f, 0.07f, 0.04f, 1.00f);
	ImVec4 darkChild = ImVec4(0.11f, 0.09f, 0.04f, 1.00f);
	ImVec4 darkPopup = ImVec4(0.12f, 0.10f, 0.05f, 1.00f);
	ImVec4 darkBorder = ImVec4(0.35f, 0.27f, 0.05f, 1.00f);

	ImVec4* colors = ImGui::GetStyle().Colors;

	colors[ImGuiCol_WindowBg] = dark;
	colors[ImGuiCol_ChildBg] = darkChild;
	colors[ImGuiCol_PopupBg] = darkPopup;
	colors[ImGuiCol_Border] = darkBorder;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 0.95f, 0.75f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.50f, 0.35f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.13f, 0.05f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = yellowHover;
	colors[ImGuiCol_FrameBgActive] = yellowActive;
	colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.09f, 0.02f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.25f, 0.18f, 0.02f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = dark;
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.09f, 0.02f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.04f, 0.02f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = yellow;
	colors[ImGuiCol_ScrollbarGrabHovered] = yellowHover;
	colors[ImGuiCol_ScrollbarGrabActive] = yellowActive;
	colors[ImGuiCol_Button] = ImVec4(0.45f, 0.32f, 0.02f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = yellowHover;
	colors[ImGuiCol_ButtonActive] = yellowActive;
	colors[ImGuiCol_Header] = ImVec4(0.45f, 0.32f, 0.02f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = yellowHover;
	colors[ImGuiCol_HeaderActive] = yellowActive;
	colors[ImGuiCol_CheckMark] = yellowActive;
	colors[ImGuiCol_SliderGrab] = yellow;
	colors[ImGuiCol_SliderGrabActive] = yellowActive;
	colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.18f, 0.02f, 1.00f);
	colors[ImGuiCol_TabHovered] = yellowHover;
	colors[ImGuiCol_TabActive] = ImVec4(0.55f, 0.40f, 0.02f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = dark;
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.30f, 0.22f, 0.02f, 1.00f);
	colors[ImGuiCol_Separator] = darkBorder;
	colors[ImGuiCol_SeparatorHovered] = yellowHover;
	colors[ImGuiCol_SeparatorActive] = yellowActive;
	colors[ImGuiCol_ResizeGrip] = yellow;
	colors[ImGuiCol_ResizeGripHovered] = yellowHover;
	colors[ImGuiCol_ResizeGripActive] = yellowActive;
	colors[ImGuiCol_PlotLines] = yellow;
	colors[ImGuiCol_PlotLinesHovered] = yellowActive;
	colors[ImGuiCol_PlotHistogram] = yellow;
	colors[ImGuiCol_PlotHistogramHovered] = yellowActive;
	colors[ImGuiCol_NavHighlight] = yellow;
	colors[ImGuiCol_NavWindowingHighlight] = yellow;

	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowRounding = 6.0f;
	style.ChildRounding = 4.0f;
	style.FrameRounding = 4.0f;
	style.PopupRounding = 4.0f;
	style.ScrollbarRounding = 4.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 4.0f;
	style.WindowBorderSize = 1.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.WindowPadding = ImVec2(10.0f, 10.0f);
	style.FramePadding = ImVec2(6.0f, 4.0f);
	style.ItemSpacing = ImVec2(8.0f, 6.0f);

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
	vkDestroyDescriptorPool(mDevice, mDescriptorPool, 0);
	mDescriptorPool = 0;

	mDevice = 0;
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
	ImGui::SliderFloat("render scale", &renderScale, 0.250f, 1.0f);
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
