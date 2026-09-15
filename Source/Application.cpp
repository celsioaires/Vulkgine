#include "Application.h"

#include <SDL.h>

void Application::initializeWindow()
{
	SDL_assert(SDL_Init(SDL_INIT_VIDEO) == 0);

	int position = SDL_WINDOWPOS_CENTERED;
	Uint32 flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN;

	mWindow.pointer = SDL_CreateWindow("Vulkgine", position, position, mWindow.width, mWindow.height, flags);
	SDL_assert(mWindow.pointer != NULL);

	mEvent.mPointer = new SDL_Event;
}

void Application::cleanupInitialized()
{
	delete mEvent.mPointer;

	SDL_DestroyWindow(mWindow.pointer);
	SDL_Quit();
}

void Application::showWindow()
{
	SDL_ShowWindow(mWindow.pointer);
}

bool Application::pollEvents(Event& event)
{
	while (SDL_PollEvent(mEvent.mPointer))
	{
		event.mPointer = mEvent.mPointer;

		// Process events
		SDL_Event e = *mEvent.mPointer;

		switch (e.type)
		{
		case SDL_WINDOWEVENT:
			switch (e.window.event)
			{
			case SDL_WINDOWEVENT_MINIMIZED:
				mMinimized = true;
				break;
			case SDL_WINDOWEVENT_RESTORED:
				mMinimized = false;
				break;
			}
			break;
		case SDL_QUIT:
			mQuit = true;
			break;
		}

		return true;
	}

	return false;
}
