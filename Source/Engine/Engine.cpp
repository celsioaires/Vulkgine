#include "Engine.h"

void Engine::initializeRenderer(Window win)
{
	mRenderer.initializeContext(win.pointer, win.width, win.height);
	mRenderer.initializePipeline();
}

void Engine::initializeGui()
{
	Context ctx = mRenderer.getContext();

	mGui.initializeDescriptors(ctx.mDevice);
	mGui.initializeContext(ctx.mWindow, ctx.instance, ctx.physicalDevice, ctx.graphicsQueue);
}

void Engine::initializeScene()
{
	mScene.initializeMeshes(mRenderer);
}

void Engine::cleanupInitialized()
{
	mRenderer.waitForRender();

	mScene.cleanupInitialized();
	mGui.cleanupInitialized();
	mRenderer.cleanupInitialized();
}

void Engine::updateGui(Event e)
{
	mGui.updatePanels(e.mPointer);
	mGui.drawPanels(mRenderer.getComputeEffect(), mRenderer.getRenderScale());
}

void Engine::drawFrame()
{
	if (mRenderer.resizeRequested())
		mRenderer.resizeSwapchain();

	VkCommandBuffer renderingBuffer = mRenderer.beginRender();

	// Scene
	mRenderer.beginScene(renderingBuffer);
	mRenderer.renderScene(renderingBuffer, mScene);
	mRenderer.endScene(renderingBuffer);

	// Gui
	mGui.renderPanels(renderingBuffer, mRenderer.getSwapchainView(), mRenderer.getSwapchainExtent());

	mRenderer.endRender();
}
