#ifndef __COMMANDER_H_INCLUDED__
#define __COMMANDER_H_INCLUDED__
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

#include "util.h"


class Commander {
    VkCommandPool pool;
    std::vector<VkCommandBuffer> buffers;
    VkQueue graphicsQueue;

    VkCommandBuffer beginSingleTimeCommands(Device device);
    void endSingleTimeCommands(Device device, VkCommandBuffer commandBuffer);

public:
    void transitionImageLayout(Device device, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
};

#endif