#include "Engine.h"

void Engine::initializeRenderer(Window window)
{
	mRenderer.initializeContext(window.pointer, window.width, window.height);
	mRenderer.initializePipeline();
}

void Engine::initializeGui()
{
	Context context = mRenderer.getContext();

	mGui.initializeDescriptors(context.mDevice);
	mGui.initializeContext(context.mWindow, context.instance, context.physicalDevice, context.graphicsQueue);
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

void Engine::updateGui(Event event)
{
	mGui.updatePanels(event.mPointer);
	mGui.drawPanels(mRenderer.getComputeEffect());
}

void Engine::drawFrame()
{
	VkCommandBuffer renderingBuffer = mRenderer.beginRender();

	// TODO: separate dynamic rendering
	// TODO: begin dynamic rendering for scene with color and depth attach
	// TODO: begin dynamic rendering for gui with only color attach

	mRenderer.renderScene(renderingBuffer, mScene);
	mGui.renderPanels(renderingBuffer);

	mRenderer.endRender();
}
