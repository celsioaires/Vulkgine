#pragma once

#include "Core/Window.h"
#include "Core/Event.h"
#include "Core/Info.h"
#include "Gui/Gui.h"
#include "Render/Renderer.h"
#include "Scene/Scene.h"

class Engine
{
private:
	Renderer mRenderer{};
	Scene mScene{};
	Gui mGui{};
	Info mInfo{};
public:
	void initializeRenderer(Window window);
	void initializeScene();
	void initializeGui();

	void cleanupInitialized();

	void processEvents(Event event);

	void prepareFrame();
	void drawFrame();
};
