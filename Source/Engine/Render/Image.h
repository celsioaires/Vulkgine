#pragma once

#include "Fwd.h"

struct Image
{
    VkDevice mDevice{};
    VmaAllocator mAllocator{};
    VkImage handle{};
    VkImageView view{};
    VmaAllocation allocation{};
    Extent3D extent{};
    VkFormat mFormat{};

    void initialize(VmaAllocator allocator, VkFormat format, VkImageUsageFlags usages, uint32_t width, uint32_t height);
    void initializeView(VkDevice device, VkImageAspectFlags aspect);

    void cleanupInitialized();
};
