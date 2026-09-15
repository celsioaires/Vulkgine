#include "Renderer.h"

#include <cmath>

#include <vulkan/vulkan.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Scene/Scene.h"
#include "Util.h"

void Renderer::initializeContext(SDL_Window* window, uint32_t width, uint32_t height)
{
	mContext.initializeDevice(window);
	mContext.initializeAllocator();
	mContext.initializeSwapchain(width, height);
	mContext.initializeCommands();
	mContext.initializeSyncronization();
}

void Renderer::initializePipeline()
{
	mPipeline.initializeDescriptors(mContext.mDevice, mContext.renderImage.view);
	mPipeline.initializeShaders();
	mPipeline.initializeCompute();
	mPipeline.initializeGraphics(mContext.instance);
}

void Renderer::cleanupInitialized()
{
	mPipeline.cleanupInitialized();
	mContext.cleanupInitialized();
}

VkCommandBuffer Renderer::beginRender()
{
	mFrame = mContext.frames[mFrameNumber % FRAME_OVERLAP];
	mRenderImage = mContext.renderImage;
	mDepthImage = mContext.mDepthImage;

	mExtent.width = mRenderImage.extent.width;
	mExtent.height = mRenderImage.extent.height;

	// Acquire swapchain image
	VkDevice device = mContext.mDevice;
	VkFence frameFence = mFrame.frameFence;

	VK_ASSERT(vkWaitForFences(device, 1, &frameFence, true, 1000000000));
	VK_ASSERT(vkResetFences(device, 1, &frameFence));

	VK_ASSERT(vkAcquireNextImageKHR(device, mContext.swapchain, 1000000000, mFrame.acquireSemaphore, NULL, &mSwapchainImageIndex));

	// Begin command buffer
	VkCommandBuffer commandBuffer = mFrame.commandBuffer;

	VK_ASSERT(vkResetCommandBuffer(commandBuffer, 0));

	VkCommandBufferBeginInfo commandBufferBeginInfo{};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VK_ASSERT(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));

	// Transition image for clear and compute
	Util::cmdTransitionImage(commandBuffer, mRenderImage.handle, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

	/* Clear image color
	VkClearColorValue clearColor{ 0.0f, 0.0f, 0.0f, 1.0f };
	VkImageSubresourceRange imageSubresourceRange = Util::imageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT);

	vkCmdClearColorImage(commandBuffer, mImage.handle, VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1, &imageSubresourceRange); */

	// Push constants and draw compute gradient grid 
	VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_COMPUTE;
	VkPipelineLayout computeLayout = mPipeline.mComputePipelineLayout;

	vkCmdBindPipeline(commandBuffer, bindPoint, mPipeline.mComputePipeline);
	vkCmdBindDescriptorSets(commandBuffer, bindPoint, computeLayout, 0, 1, &mPipeline.mDescriptorSet, 0, NULL);
	vkCmdPushConstants(commandBuffer, computeLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(ComputePushConstants), &mComputeEffect);
	vkCmdDispatch(commandBuffer, (uint32_t)std::ceil(mExtent.width / 16.0), (uint32_t)std::ceil(mExtent.height / 16.0), 1);
	
	// Transition image for rendering
	Util::cmdTransitionImage(commandBuffer, mRenderImage.handle, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	Util::cmdTransitionImage(commandBuffer, mDepthImage.handle, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);

	// Color attachment
	VkRenderingAttachmentInfo colorAttachmentInfo{};
	colorAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
	colorAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	colorAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentInfo.imageView = mRenderImage.view;

	// Depth attachment
	VkRenderingAttachmentInfo depthAttachmentInfo{};
	depthAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
	depthAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
	depthAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depthAttachmentInfo.imageView = mDepthImage.view;
	depthAttachmentInfo.clearValue.depthStencil.depth = 1.0f;

	// Begin rendering
	VkRenderingInfo renderingInfo{};
	renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
	renderingInfo.renderArea.extent = { mExtent.width, mExtent.height };
	renderingInfo.pColorAttachments = &colorAttachmentInfo;
	renderingInfo.pDepthAttachment= &depthAttachmentInfo;
	renderingInfo.colorAttachmentCount = 1;
	renderingInfo.layerCount = 1;

	vkCmdBeginRendering(commandBuffer, &renderingInfo);

	return commandBuffer;
}

void Renderer::endRender()
{
	VkCommandBuffer commandBuffer = mFrame.commandBuffer;

	// End rendering
	vkCmdEndRendering(commandBuffer);

	// Transition image for copy
	VkImage swapchainImage = mContext.swapchainImages[mSwapchainImageIndex];

	Util::cmdTransitionImage(commandBuffer, mRenderImage.handle, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
	Util::cmdTransitionImage(commandBuffer, swapchainImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	// Copy image to swapchain image
	Util::cmdCopyImageToImage(commandBuffer, mRenderImage.handle, swapchainImage, { mExtent.width, mExtent.height }, { mContext.swapchainExtent.width, mContext.swapchainExtent.height });

	// Transition swapchain image for present
	Util::cmdTransitionImage(commandBuffer, swapchainImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

	// End command buffer
	VK_ASSERT(vkEndCommandBuffer(commandBuffer));

	// Submit queue
	VkCommandBufferSubmitInfo commandBufferSubmitInfo{};
	commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
	commandBufferSubmitInfo.commandBuffer = commandBuffer;

	VkSemaphoreSubmitInfo waitSemaphoreInfo =
		Util::semaphoreSubmitInfo(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, mFrame.acquireSemaphore);

	VkSemaphoreSubmitInfo signalSemaphoreInfo =
		Util::semaphoreSubmitInfo(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, mFrame.submitSemaphore);

	VkSubmitInfo2 submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
	submitInfo.pCommandBufferInfos = &commandBufferSubmitInfo;
	submitInfo.pWaitSemaphoreInfos = &waitSemaphoreInfo;
	submitInfo.pSignalSemaphoreInfos = &signalSemaphoreInfo;
	submitInfo.commandBufferInfoCount = 1;
	submitInfo.waitSemaphoreInfoCount = 1;
	submitInfo.signalSemaphoreInfoCount = 1;
	
	VK_ASSERT(vkQueueSubmit2(mContext.graphicsQueue, 1, &submitInfo, mFrame.frameFence));

	// Present queue
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pImageIndices = &mSwapchainImageIndex;
	presentInfo.pSwapchains = &mContext.swapchain;
	presentInfo.pWaitSemaphores = &mFrame.submitSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.waitSemaphoreCount = 1;

	VK_ASSERT(vkQueuePresentKHR(mContext.graphicsQueue, &presentInfo));

	mFrameNumber++;
}

void Renderer::renderScene(VkCommandBuffer commandBuffer, Scene& scene)
{
	// Data
	std::vector<MeshAsset> meshAssets = scene.getMeshes();

	MeshAsset meshAsset = meshAssets[2];
	Geometry geometry = meshAsset.mGeometries[0];
	Mesh mesh = meshAsset.mMesh;

	VkViewport viewport{};
	viewport.width = (float)mExtent.width;
	viewport.height = (float)mExtent.height;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.extent = { mExtent.width, mExtent.height };

	glm::mat4 view = glm::translate(glm::mat4(1), { 0, 0, -3 });
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)mExtent.width / (float)mExtent.height, 0.1f, 10000.0f); // TODO: change to near=10000.0f, far=0.1f
	
	projection[1][1] *= -1;

	GraphicsPushConstants pushConstants{};
	pushConstants.viewProjection = projection * view;
	pushConstants.mVertexBuffer = mesh.mVertexBuffer.mAddress;

	// Commands
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline.mGraphicsPipeline);
	
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	vkCmdPushConstants(commandBuffer, mPipeline.mGraphicsPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(GraphicsPushConstants), &pushConstants);

	vkCmdBindIndexBuffer(commandBuffer, mesh.mIndexBuffer.mBuffer, 0, VK_INDEX_TYPE_UINT32);
	vkCmdDrawIndexed(commandBuffer, geometry.mIndexCount, 1, geometry.mStartIndex, 0, 0);
}

VkCommandBuffer Renderer::beginImmediateRender()
{
	VkCommandBuffer commandBuffer = mContext.mImmediateCommandBuffer;

	VK_ASSERT(vkResetFences(mContext.mDevice, 1, &mContext.mImmediateFence));
	VK_ASSERT(vkResetCommandBuffer(commandBuffer, 0));

	VkCommandBufferBeginInfo commandBufferInfo{};
	commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VK_ASSERT(vkBeginCommandBuffer(commandBuffer, &commandBufferInfo));

	return commandBuffer;
}

void Renderer::endImmediateRender(VkCommandBuffer commandBuffer)
{
	VK_ASSERT(vkEndCommandBuffer(commandBuffer));

	VkCommandBufferSubmitInfo commandBufferInfo{};
	commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
	commandBufferInfo.commandBuffer = commandBuffer;

	VkSubmitInfo2 submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
	submitInfo.pCommandBufferInfos = &commandBufferInfo;
	submitInfo.commandBufferInfoCount = 1;

	VkFence fence = mContext.mImmediateFence;

	VK_ASSERT(vkQueueSubmit2(mContext.graphicsQueue, 1, &submitInfo, fence));
	VK_ASSERT(vkWaitForFences(mContext.mDevice, 1, &fence, true, 9999999999));
}

void Renderer::waitForRender()
{
	VK_ASSERT(vkDeviceWaitIdle(mContext.mDevice));
}
