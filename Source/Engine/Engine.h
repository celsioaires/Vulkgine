#pragma once

#include "Render/Renderer.h"
#include "Gui/Gui.h"
#include "Scene/Scene.h"
#include "Core/Window.h"
#include "Core/Event.h"

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

	void drawFrame();
};
