#pragma once

#include "Image.h"

class Renderer;

struct Texture
{
	Image mImage{};
	VkSampler mSampler{};

	void initializeImage(Renderer& renderer, void* pixels, uint32_t width, uint32_t height);
	void initializeWhiteImage(Renderer& renderer);

	void cleanupInitialized();
};
