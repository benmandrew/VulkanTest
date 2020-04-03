#ifndef __UTIL_H_INCLUDED__
#define __UTIL_H_INCLUDED__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <cstring>
#include <iostream>

#include "instance.h"


const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
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

bool checkValidationLayerSupport();

void populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT& createInfo);

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator);

std::vector<const char*> getRequiredExtensions(
    bool validationLayersEnabled);

bool checkDeviceExtensionSupport(
    VkPhysicalDevice device);

#endif