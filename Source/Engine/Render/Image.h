#pragma once

#include "Fwd.h"

struct Image
{
    VkImage handle;
    VkImageView view;
    VmaAllocation allocation;
    Extent3D extent;
    VkFormat format;
};
