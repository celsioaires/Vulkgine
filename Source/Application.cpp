#include "Application.h"

#include <SDL2/SDL.h>

void Application::initializeWindow()
{
	int position = SDL_WINDOWPOS_CENTERED;
	
	Uint32 flags = 
		SDL_WINDOW_HIDDEN |
		SDL_WINDOW_RESIZABLE |
		SDL_WINDOW_VULKAN;
	
	SDL_assert(SDL_Init(SDL_INIT_VIDEO) == 0);

	mWindow.mPointer = SDL_CreateWindow("Vulkgine", position, position, mWindow.mWidth, mWindow.mHeight, flags);
	mEvent.mPointer = new SDL_Event;

	SDL_assert(mWindow.mPointer);
	SDL_assert(mEvent.mPointer);
}

void Application::cleanupInitialized()
{
	delete mEvent.mPointer;

	SDL_DestroyWindow(mWindow.mPointer);
	SDL_Quit();
}

void Application::showWindow()
{
	SDL_ShowWindow(mWindow.mPointer);
}

bool Application::pollEvents(Event& event)
{
	// Event loop
	while (SDL_PollEvent(mEvent.mPointer))
	{
		event.mPointer = mEvent.mPointer;

		// Poll events
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
