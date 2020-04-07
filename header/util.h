#ifndef __UTIL_H_INCLUDED__
#define __UTIL_H_INCLUDED__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <stdexcept>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <optional>
#include <set>
#include <array>
#include <chrono>
#include <string>


const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


struct Device;
struct Instance;


struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


void createImage(
    Device* device,
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

VkImageView createImageView(
    Device* device,
    VkImage image,
    VkFormat format,
    VkImageAspectFlags aspectFlags,
    uint32_t mipLevels);

void createBuffer(
    Device* device,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer& buffer,
    VkDeviceMemory& bufferMemory);

uint32_t findMemoryType(
    Device* device,
    uint32_t typeFilter,
    VkMemoryPropertyFlags properties);

VkFormat findSupportedFormat(
    Device* device,
    const std::vector<VkFormat>& candidates,
    VkImageTiling tiling,
    VkFormatFeatureFlags features);

VkFormat findDepthFormat(
    Device* device);

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

QueueFamilyIndices findQueueFamilies(
    Instance* instance,
    VkPhysicalDevice device);

SwapChainSupportDetails querySwapChainSupport(
    Instance* instance);

static std::vector<char> readFile(
    const std::string& filename);

VkShaderModule createShaderModule(
    Device* device, 
    const std::vector<char>& code);

#endif