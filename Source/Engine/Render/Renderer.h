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
public:
	void initializeContext(SDL_Window* window, uint32_t width, uint32_t height);
	void initializePipeline();

	void cleanupInitialized();

	VkCommandBuffer beginRender();
	void endRender();

	void renderScene(VkCommandBuffer commandBuffer, Scene& scene);

	VkCommandBuffer beginImmediateRender();
	void endImmediateRender(VkCommandBuffer commandBuffer);

	void waitForRender();

	// Getters
	Context& getContext() { return mContext; }
	ComputeEffect& getComputeEffect() { return mComputeEffect; }
};
