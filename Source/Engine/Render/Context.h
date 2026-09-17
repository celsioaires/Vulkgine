#pragma once

#include <vector>

#include "Frame.h"
#include "Image.h"

struct Context
{
	SDL_Window* mWindow = NULL;
	VkInstance instance = NULL;
	VkDebugUtilsMessengerEXT debugMessenger = NULL;
	VkSurfaceKHR surface = NULL;
	VkPhysicalDevice physicalDevice = NULL;
	VkDevice mDevice = NULL;
	VkQueue graphicsQueue = NULL;
	uint32_t graphicsQueueIndex = UINT32_MAX;
	VmaAllocator allocator = NULL;
	VkSwapchainKHR swapchain = NULL;
	Extent2D swapchainExtent{ 0, 0 };
	std::vector<VkImage> swapchainImages{};
	std::vector<VkImageView> mSwapchainViews{};
	Image renderImage;
	Image mDepthImage;
	Frame frames[FRAME_OVERLAP];
	VkCommandPool mImmediateCommandPool = NULL;
	VkCommandBuffer mImmediateCommandBuffer = NULL;
	VkFence mImmediateFence = NULL;

	void initializeDevice(SDL_Window* window);
	void initializeAllocator();
	void initializeSwapchain(uint32_t width, uint32_t height);
	void initializeCommands();
	void initializeSyncronization();

	void cleanupInitialized();
	void cleanupSwapchain();
};
