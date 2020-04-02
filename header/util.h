#ifndef __UTIL_H_INCLUDED__
#define __UTIL_H_INCLUDED__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

#include "commander.h"

class Commander;

struct Instance {
    Device device;
    Commander* commander;
};


void createImage(
    Device device,
    uint32_t width,
    uint32_t height,
    uint32_t mipLevels,
    VkSampleCountFlagBits numSamples,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkImage& image, 
    VkDeviceMemory& imageMemory);

void createBuffer(
    Device device,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer& buffer,
    VkDeviceMemory& bufferMemory);

uint32_t findMemoryType(
    Device device,
    uint32_t typeFilter,
    VkMemoryPropertyFlags properties);

bool hasStencilComponent(
    VkFormat format);

#endif