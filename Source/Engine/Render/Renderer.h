#pragma once

#include "Context.h"
#include "Frame.h"
#include "Pipeline.h"

class Scene;

class Renderer
{
private:
	Context mContext;
	Frame mFrame;
	Image mRenderImage;
	Image mDepthImage;
	int mFrameNumber = 0;
	uint32_t mSwapchainImageIndex = UINT32_MAX;
	Extent2D mExtent;
	Pipeline mPipeline;
	ComputeEffect mComputeEffect;
	bool mResizeRequested = false;
	float mRenderScale = 1.0f;
public:
	// Initialize
	void initializeContext(SDL_Window* window, uint32_t width, uint32_t height);
	void initializePipeline();

	void cleanupInitialized();

	void resizeSwapchain();

	// Render
	VkCommandBuffer beginRender();
	void endRender();

	// Scene
	void beginScene(VkCommandBuffer commandBuffer);
	void endScene(VkCommandBuffer commandBuffer);
	void renderScene(VkCommandBuffer commandBuffer, Scene& scene);
	
	// Immediate
	VkCommandBuffer beginImmediateRender();
	void endImmediateRender(VkCommandBuffer commandBuffer);

	void waitForRender();

	// Getters
	Context& getContext() { return mContext; }
	ComputeEffect& getComputeEffect() { return mComputeEffect; }
	bool resizeRequested() { return mResizeRequested; }
	float& getRenderScale() { return mRenderScale; }
	VkImageView getSwapchainView() { return mContext.mSwapchainViews[mSwapchainImageIndex]; }
	Extent2D getSwapchainExtent() { return mContext.swapchainExtent; }
};
