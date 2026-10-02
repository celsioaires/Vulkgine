#include "Engine.h"

void Engine::initializeRenderer(Window window)
{
	mRenderer.initializeContext(window.mPointer, window.mWidth, window.mHeight);
	mRenderer.initializePipeline();
	mRenderer.initializeCamera();
}

void Engine::initializeScene()
{
	mScene.initializeMeshes(mRenderer);
	mScene.initializeNodes();
}

void Engine::initializeGui()
{
	Context renderingContext = mRenderer.getContext();
	std::vector<Node*>& sceneNodes = mScene.getNodes();

	mGui.initializeDescriptors(renderingContext.mDevice);
	mGui.initializeContext(renderingContext.mWindow, renderingContext.instance, renderingContext.physicalDevice, renderingContext.graphicsQueue);
	mGui.initializePanels(sceneNodes[0]);
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
	mGui.drawPanels(mScene.getNodes(), mRenderer.getComputeEffect(), mRenderer.getRenderScale());
}

void Engine::prepareFrame()
{
	// Resizing
	if (mRenderer.resizeRequested())
		mRenderer.resizeSwapchain();

	mRenderer.clearRenderables();
	mScene.submitRenderables(mRenderer);

	mRenderer.updateScene();
}

void Engine::drawFrame()
{
	VkCommandBuffer renderingBuffer = mRenderer.beginRender();

	// Scene
	mRenderer.beginScene(renderingBuffer);
	mRenderer.renderRenderables(renderingBuffer);
	mRenderer.endScene(renderingBuffer);

	// Gui
	mGui.renderPanels(renderingBuffer, mRenderer.getSwapchainView(), mRenderer.getSwapchainExtent());

	mRenderer.endRender();
}
