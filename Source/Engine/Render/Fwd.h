#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

#include <glm/glm.hpp>

// SDL
typedef struct SDL_Window SDL_Window;
typedef union SDL_Event SDL_Event;

// Vulkan
#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;

typedef uint32_t VkFlags;
typedef uint64_t VkFlags64;
typedef uint64_t VkDeviceSize;
typedef uint64_t VkDeviceAddress;

typedef VkFlags VkBufferUsageFlags;
typedef VkFlags VkImageAspectFlags;
typedef VkFlags VkShaderStageFlags;
typedef VkFlags VkImageUsageFlags;
typedef VkFlags64 VkPipelineStageFlags2;

VK_DEFINE_HANDLE(VkInstance)
VK_DEFINE_HANDLE(VkDebugUtilsMessengerEXT)
VK_DEFINE_HANDLE(VkSurfaceKHR)
VK_DEFINE_HANDLE(VkPhysicalDevice)
VK_DEFINE_HANDLE(VkDevice)
VK_DEFINE_HANDLE(VkSwapchainKHR)
VK_DEFINE_HANDLE(VkQueue)
VK_DEFINE_HANDLE(VkCommandPool)
VK_DEFINE_HANDLE(VkCommandBuffer)
VK_DEFINE_HANDLE(VkSemaphore)
VK_DEFINE_HANDLE(VkFence)
VK_DEFINE_HANDLE(VkImage)
VK_DEFINE_HANDLE(VkImageView)
VK_DEFINE_HANDLE(VkDescriptorPool)
VK_DEFINE_HANDLE(VkDescriptorSetLayout)
VK_DEFINE_HANDLE(VkDescriptorSet)
VK_DEFINE_HANDLE(VkPipeline)
VK_DEFINE_HANDLE(VkPipelineLayout)

// non dispatchable
VK_DEFINE_HANDLE(VkShaderModule)
VK_DEFINE_HANDLE(VkBuffer)
VK_DEFINE_HANDLE(VkSampler)

struct VkImageSubresourceRange;
struct VkSemaphoreSubmitInfo;

enum VkImageLayout;
enum VkFormat;
enum VkDescriptorType;

// Vulkan Memory Allocator
VK_DEFINE_HANDLE(VmaAllocator)
VK_DEFINE_HANDLE(VmaAllocation)

enum VmaMemoryUsage;

// Extent
struct Extent2D
{
    uint32_t width;
    uint32_t height;
};

struct Extent3D
{
    uint32_t width;
    uint32_t height;
    uint32_t depth;
};

struct ComputePushConstants
{
    glm::vec4 data1{};
    glm::vec4 data2{};
    glm::vec4 data3{};
    glm::vec4 data4{};
};

struct ComputeEffect
{
    ComputePushConstants mPushConstants;
};

struct GraphicsPushConstants
{
    glm::mat4 viewProjection = glm::mat4(1);
    VkDeviceAddress mVertexBuffer = 0;
};

struct DescriptorPoolSize
{
    VkDescriptorType mType;
    uint32_t mDescriptorCount;
};
