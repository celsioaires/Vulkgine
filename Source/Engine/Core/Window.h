#pragma once

#include <cstddef>

struct SDL_Window;

struct Window
{
	SDL_Window* mPointer = NULL;
	int mWidth = 1280;
	int mHeight = 720;
};
