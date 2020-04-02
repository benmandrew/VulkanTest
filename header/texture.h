#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <string>
#include <stdexcept>

#include "util.h"


class Texture {
    VkImage image;
    VkDeviceMemory memory;
    VkImageView view;
    VkSampler sampler;
    uint32_t mipLevels;

    void create(Instance instance, std::string imgPath);
};

#endif