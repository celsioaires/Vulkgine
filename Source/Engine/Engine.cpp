#include "Engine.h"

void Engine::initializeRenderer(Window window)
{
	mRenderer.initializeContext(window.mPointer, window.mWidth, window.mHeight);
	mRenderer.initializePipeline();
	mRenderer.initializeCamera();
}

void Engine::initializeGui()
{
	Context renderingContext = mRenderer.getContext();

	mGui.initializeDescriptors(renderingContext.mDevice);
	mGui.initializeContext(renderingContext.mWindow, renderingContext.instance, renderingContext.physicalDevice, renderingContext.graphicsQueue);
}

void Engine::initializeScene()
{
	mScene.initializeMeshes(mRenderer);
	mScene.initializeTextures(mRenderer);
}

void Engine::cleanupInitialized()
{
	mRenderer.waitForRender();

	mScene.cleanupInitialized();
	mGui.cleanupInitialized();
	mRenderer.cleanupInitialized();
}

void Engine::updateGui(Event event)
{
	mGui.updatePanels(event.mPointer);
	mGui.drawPanels(mRenderer.getComputeEffect(), mRenderer.getRenderScale());
}

void Engine::beginFrame()
{
	// Resizing
	if (mRenderer.resizeRequested())
		mRenderer.resizeSwapchain();
}

void Engine::endFrame()
{
	mRenderer.endRender();
}

void Engine::drawFrame()
{
	VkCommandBuffer renderingBuffer = mRenderer.beginRender();

	// Scene
	mRenderer.beginScene(renderingBuffer);
	mRenderer.renderScene(renderingBuffer, mScene);
	mRenderer.endScene(renderingBuffer);

	// Gui
	mGui.renderPanels(renderingBuffer, mRenderer.getSwapchainView(), mRenderer.getSwapchainExtent());
}
