#include "Camera.h"

#include <SDL2/SDL_events.h>
#include <vma/vk_mem_alloc.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

void Camera::initializeUbo(VkDevice device, VmaAllocator allocator)
{
	mUbo.initialize(device, allocator, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, sizeof(mMvp), VMA_MEMORY_USAGE_CPU_TO_GPU);
}

void Camera::cleanupInitialized()
{
	mUbo.cleanup();
}

void Camera::processMovement(SDL_Event* event)
{
	SDL_Event& e = *event;

	if (e.type == SDL_KEYDOWN) 
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
			mVelocity.z = -1;
			break;
		case SDLK_s:
			mVelocity.z = 1;
			break;
		case SDLK_a:
			mVelocity.x = -1;
			break;
		case SDLK_d:
			mVelocity.x = 1;
			break;
		}
	}

	if (e.type == SDL_KEYUP) {
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
			mVelocity.z = 0;
			break;
		case SDLK_s:
			mVelocity.z = 0;
			break;
		case SDLK_a:
			mVelocity.x = 0;
			break;
		case SDLK_d:
			mVelocity.x = 0;
			break;
		}
	}

	if (e.type == SDL_MOUSEMOTION)
	{
		mYaw += (float)e.motion.xrel / 200.f;
		mPitch -= (float)e.motion.yrel / 200.f;
	}
}

void Camera::updateData()
{
	glm::mat4 rotation = getRotation();
	mPosition += glm::vec3(rotation * glm::vec4(mVelocity * 0.5f, 0.f));
}

glm::mat4 Camera::getView()
{
	glm::mat4 translation = glm::translate(glm::mat4(1), mPosition);
	glm::mat4 rotation = getRotation();

	return glm::inverse(translation * rotation);
}

glm::mat4 Camera::getRotation()
{
	glm::quat pitch = glm::angleAxis(mPitch, glm::vec3(1, 0, 0));
	glm::quat yaw = glm::angleAxis(mYaw, glm::vec3(0, -1, 0));

	return glm::toMat4(yaw) * glm::toMat4(pitch);
}
