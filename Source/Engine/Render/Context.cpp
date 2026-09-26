#include "Context.h"

#include <fmt/core.h>
#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_vulkan.h>
#include <VkBootstrap.h>
#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

#include "Util.h"

void Context::initializeDevice(SDL_Window* window)
{
	mWindow = window;

#ifdef _DEBUG
	bool enableValidation = true;
#else
	bool enableValidation = false;
#endif

	// Instance
	vkb::InstanceBuilder instanceBuilder;

	vkb::Result<vkb::Instance> instanceResult = instanceBuilder
		.enable_extension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
		.set_app_name("Vulkgine")
		.request_validation_layers(enableValidation)
		.use_default_debug_messenger()
		.require_api_version(1, 3)
		.build();

	assert(instanceResult.has_value());

	vkb::Instance& vkbInstance = instanceResult.value();
	instance = vkbInstance.instance;
	debugMessenger = vkbInstance.debug_messenger;

	// Surface
	SDL_assert(SDL_Vulkan_CreateSurface(window, instance, &surface));

	// Physical device
	VkPhysicalDeviceVulkan12Features physicalDeviceFeatures12{};
	physicalDeviceFeatures12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
	physicalDeviceFeatures12.bufferDeviceAddress = true;
	physicalDeviceFeatures12.descriptorIndexing = true;

	VkPhysicalDeviceVulkan13Features physicalDeviceFeatures13{};
	physicalDeviceFeatures13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
	physicalDeviceFeatures13.dynamicRendering = true;
	physicalDeviceFeatures13.synchronization2 = true;

	vkb::PhysicalDeviceSelector physicalDeviceSelector(vkbInstance);

	vkb::Result<vkb::PhysicalDevice> physicalDeviceResult = physicalDeviceSelector
		.set_minimum_version(1, 3)
		.set_required_features_13(physicalDeviceFeatures13)
		.set_required_features_12(physicalDeviceFeatures12)
		.set_surface(surface)
		.select();

	assert(physicalDeviceResult.has_value());

	vkb::PhysicalDevice& vkbPhysicalDevice = physicalDeviceResult.value();
	physicalDevice = vkbPhysicalDevice.physical_device;

	// Device
	vkb::DeviceBuilder deviceBuilder(vkbPhysicalDevice);

	vkb::Result<vkb::Device> deviceResult = deviceBuilder.build();
	assert(deviceResult.has_value());

	vkb::Device& vkbDevice = deviceResult.value();
	mDevice = vkbDevice.device;

	// Graphics queue
	vkb::Result<VkQueue> graphicsQueueResult = vkbDevice.get_queue(vkb::QueueType::graphics);
	assert(graphicsQueueResult.has_value());

	graphicsQueue = graphicsQueueResult.value();

	// Graphics queue index
	vkb::Result<uint32_t> graphicsQueueIndexResult = vkbDevice.get_queue_index(vkb::QueueType::graphics);
	assert(graphicsQueueIndexResult.has_value());

	graphicsQueueIndex = graphicsQueueIndexResult.value();
}

void Context::initializeAllocator()
{
	VmaAllocatorCreateInfo allocatorInfo{};
	allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
	allocatorInfo.instance = instance;
	allocatorInfo.physicalDevice = physicalDevice;
	allocatorInfo.device = mDevice;

	VK_ASSERT(vmaCreateAllocator(&allocatorInfo, &allocator));
}

void Context::initializeSwapchain(uint32_t width, uint32_t height)
{
	// Surface
	VkSurfaceFormatKHR surfaceFormat{};
	surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;

	// Usages
	VkImageUsageFlags imageUsages =
		VK_IMAGE_USAGE_TRANSFER_DST_BIT |
		VK_IMAGE_USAGE_STORAGE_BIT |
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	// Builder
	vkb::SwapchainBuilder swapchainBuilder(physicalDevice, mDevice, surface);

	// Create swapchain
	vkb::Result<vkb::Swapchain> swapchainResult = swapchainBuilder
		.set_desired_format(surfaceFormat)
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_extent(width, height)
		.add_image_usage_flags(imageUsages)
		.build();

	assert(swapchainResult.has_value());

	vkb::Swapchain& vkbSwapchain = swapchainResult.value();
	swapchain = vkbSwapchain.swapchain;
	swapchainExtent = { width, height };

	// Images
	vkb::Result<std::vector<VkImage>> swapchainImagesResult = vkbSwapchain.get_images();
	assert(swapchainImagesResult.has_value());

	swapchainImages = swapchainImagesResult.value();

	// Views
	vkb::Result<std::vector<VkImageView>> swapchainViewsResult = vkbSwapchain.get_image_views();
	assert(swapchainViewsResult.has_value());

	mSwapchainViews = swapchainViewsResult.value();

	imageUsages |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

	// Render image
	renderImage.initialize(allocator, VK_FORMAT_R16G16B16A16_SFLOAT, imageUsages, width, height);
	renderImage.initializeView(mDevice, VK_IMAGE_ASPECT_COLOR_BIT);

	imageUsages = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

	// Depth image
	mDepthImage.initialize(allocator, VK_FORMAT_D32_SFLOAT, imageUsages, width, height);
	mDepthImage.initializeView(mDevice, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void Context::initializeCommands()
{
	// Pool
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = graphicsQueueIndex;

	// Info
	VkCommandBufferAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.commandBufferCount = 1;

	// Create
	VK_ASSERT(vkCreateCommandPool(mDevice, &poolInfo, 0, &mImmediateCommandPool));

	// Allocate
	allocateInfo.commandPool = mImmediateCommandPool;

	VK_ASSERT(vkAllocateCommandBuffers(mDevice, &allocateInfo, &mImmediateCommandBuffer));
}

void Context::initializeSyncronization()
{
	// Fence
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	// Create
	VK_ASSERT(vkCreateFence(mDevice, &fenceInfo, 0, &mImmediateFence));
}

void Context::initializeFrames()
{
	mUboDescriptorLayout.initialize(mDevice, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
	mTextureDescriptorLayout.initialize(mDevice, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
	
	for (Frame& frame : frames)
	{
		frame.initializeCommands(mDevice, graphicsQueueIndex);
		frame.initializeSyncronization();
		frame.intializeDescriptors(mUboDescriptorLayout, mTextureDescriptorLayout);
	}
}

void Context::cleanupInitialized()
{
	// Frame commands and synchronization
	for (Frame& frame : frames)
		frame.cleanupInitialized();

	mTextureDescriptorLayout.cleanup();
	mUboDescriptorLayout.cleanup();

	// Syncronization
	vkDestroyFence(mDevice, mImmediateFence, 0);
	mImmediateFence = 0;

	// Commands
	vkDestroyCommandPool(mDevice, mImmediateCommandPool, 0);
	mImmediateCommandPool = 0;

	cleanupSwapchain();

	vmaDestroyAllocator(allocator);

	vkDestroyDevice(mDevice, 0);
	vkDestroySurfaceKHR(instance, surface, 0);

	vkb::destroy_debug_utils_messenger(instance, debugMessenger);

	vkDestroyInstance(instance, 0);

	allocator = 0;
	mDevice = 0;
	physicalDevice = 0;
	surface = 0;
	debugMessenger = 0;
	instance = 0;
}

void Context::cleanupSwapchain()
{
	renderImage.cleanupInitialized();
	mDepthImage.cleanupInitialized();

	for (VkImageView& view : mSwapchainViews)
	{
		vkDestroyImageView(mDevice, view, 0);
		view = 0;
	}

	vkDestroySwapchainKHR(mDevice, swapchain, 0);

	swapchain = 0;
}
