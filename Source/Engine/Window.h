#pragma once

#include <cstddef>

typedef struct SDL_Window SDL_Window;

struct Window
{
	SDL_Window* pointer = NULL;
	int width = 1280;
	int height = 720;
};
