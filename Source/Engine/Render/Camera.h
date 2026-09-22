#pragma once

#include "Buffer.h"

class Renderer;

struct Mvp
{
	glm::mat4 mModel{};
	glm::mat4 mView{};
	glm::mat4 mProjection{};
};

struct Camera
{
	Mvp mMvp{};
	Buffer mUbo{};

	void initializeUbo(VkDevice device, VmaAllocator allocator);
	void cleanupInitialized();
};
