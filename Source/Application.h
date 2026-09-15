#pragma once

#include <cstddef>

#include "Engine/Window.h"
#include "Engine/Event.h"

typedef union SDL_Event SDL_Event;

class Application
{
private:
	Window mWindow;
	Event mEvent;
	bool mQuit = false;
	bool mMinimized = false;
public:
	void initializeWindow();
	void cleanupInitialized();

	void showWindow();
	bool pollEvents(Event& event);

	bool isOpen() { return !mQuit; }
	bool isMinimized() { return mMinimized; }

	Window getWindow() const { return mWindow; }
};
