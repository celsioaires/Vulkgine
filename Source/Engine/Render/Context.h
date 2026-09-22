#pragma once

#include <vector>

#include "Frame.h"
#include "Image.h"

struct Context
{
	SDL_Window* mWindow{};
	VkInstance instance{};
	VkDebugUtilsMessengerEXT debugMessenger{};
	VkSurfaceKHR surface{};
	VkPhysicalDevice physicalDevice{};
	VkDevice mDevice{};
	VkQueue graphicsQueue{};
	uint32_t graphicsQueueIndex = UINT32_MAX;
	VmaAllocator allocator{};
	VkSwapchainKHR swapchain{};
	Extent2D swapchainExtent{};
	std::vector<VkImage> swapchainImages{};
	std::vector<VkImageView> mSwapchainViews{};
	Image renderImage{};
	Image mDepthImage{};
	Frame frames[FRAME_OVERLAP]{};
	VkCommandPool mImmediateCommandPool{};
	VkCommandBuffer mImmediateCommandBuffer{};
	VkFence mImmediateFence{};

	// Initialize
	void initializeDevice(SDL_Window* window);
	void initializeAllocator();
	void initializeSwapchain(uint32_t width, uint32_t height);
	void initializeCommands();
	void initializeSyncronization();

	// Cleanup
	void cleanupInitialized();
	void cleanupSwapchain();
};
