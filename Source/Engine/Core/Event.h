#pragma once

#include <cstddef>

typedef union SDL_Event SDL_Event;

struct Event
{
	SDL_Event* mPointer = NULL;
};
