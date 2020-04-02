#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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

    void create(Device device, std::string imgPath);
};

#endif