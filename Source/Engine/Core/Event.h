#pragma once

#include <cstddef>

union SDL_Event;

struct Event
{
	SDL_Event* mPointer = NULL;
};
