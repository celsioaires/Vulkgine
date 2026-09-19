#pragma once

#include "Engine/Core/Window.h"
#include "Engine/Core/Event.h"

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

	// Getters
	bool isOpen() { return !mQuit; }
	bool isMinimized() { return mMinimized; }
	Window getWindow() const { return mWindow; }
};
