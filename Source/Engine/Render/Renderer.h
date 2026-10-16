#pragma once

#include "Context.h"
#include "Frame.h"
#include "Pipeline.h"
#include "Camera.h"
#include "Renderable.h"

class Renderer
{
private:
	Context mContext{};
	Frame mFrame{};
	Image mRenderImage{};
	Image mDepthImage{};
	int mFrameNumber{};
	uint32_t mSwapchainImageIndex = UINT32_MAX;
	Extent2D mExtent{};
	Pipeline mPipeline{};
	ComputeEffect mComputeEffect{};
	bool mResizeRequested{};
	float mRenderScale{ 1.0f };
	Camera mCamera{};
	std::vector<Renderable> mOpaqueRenderables{};
	std::vector<Renderable> mTransparentRenderables{};
	bool mEnableTransparents{};
	int mTriangleCount{};
public:
	// Initialize
	void initializeContext(SDL_Window* window, uint32_t width, uint32_t height);
	void initializePipeline();
	void initializeCamera();

	void cleanupInitialized();

	void waitForRender();
	void resizeSwapchain();

	// Render
	VkCommandBuffer beginRender();
	void endRender();

	// Scene
	void beginScene(VkCommandBuffer commandBuffer);
	void endScene(VkCommandBuffer commandBuffer);
	void updateScene();
	
	// Renderables
	void clearRenderables();
	void submitRenderable(Renderable& renderable, bool transparent);
	void renderRenderables(VkCommandBuffer commandBuffer);

	// Camera
	void updateCamera(SDL_Event* event);

	// Immediate
	VkCommandBuffer beginImmediateRender();
	void endImmediateRender(VkCommandBuffer commandBuffer);

	// Getters
	Context& getContext() { return mContext; }
	ComputeEffect& getComputeEffect() { return mComputeEffect; }
	bool resizeRequested() { return mResizeRequested; }
	float& getRenderScale() { return mRenderScale; }
	VkImageView getSwapchainView() { return mContext.mSwapchainViews[mSwapchainImageIndex]; }
	Extent2D getSwapchainExtent() { return mContext.swapchainExtent; }
	Pipeline& getPipeline() { return mPipeline; }
	bool& getEnableTransparents() { return mEnableTransparents; }
	int getTriangleCount() { return mTriangleCount; };
};
