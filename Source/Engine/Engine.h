#pragma once

#include "Core/Window.h"
#include "Core/Event.h"
#include "Gui/Gui.h"
#include "Render/Renderer.h"
#include "Scene/Scene.h"

class Engine
{
private:
	Renderer mRenderer;
	Gui mGui;
	Scene mScene;
public:
	void initializeRenderer(Window window);
	void initializeGui();
	void initializeScene();

	void cleanupInitialized();

	void updateGui(Event event);

	void beginFrame();
	void endFrame();

	void drawFrame();
};
