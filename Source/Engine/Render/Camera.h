#pragma once

#include "Buffer.h"

class Renderer;

struct Mvp
{
	glm::mat4 mView{};
	glm::mat4 mProjection{};
};

struct Camera
{
	Mvp mMvp{};
	Buffer mUbo{};
	glm::vec3 mPosition{ glm::vec3(30.f, -00.f, 085.f) };
	glm::vec3 mVelocity{};
	float mPitch{};
	float mYaw{};

	void initializeUbo(VkDevice device, VmaAllocator allocator);
	void cleanupInitialized();

	void processMovement(SDL_Event* event);
	void updateData();

	glm::mat4 getView();
	glm::mat4 getRotation();
};
