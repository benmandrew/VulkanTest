#ifndef __SURFACE_H_INCLUDED__
#define __SURFACE_H_INCLUDED__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <optional>
#include <set>
#include <array>

#include "util.h"


struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct Surface {
    uint32_t width = 800;
    uint32_t height = 600;
    bool framebufferResized = true;
    GLFWwindow* window;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;

    void createWindow();
    void createSurface(Instance instance);
    void createSwapChain(Instance instance);
    void createImageViews(Device device);

    void destroyWindow();
    void destroySurface(Instance instance);
    void destroySwapChain(Device device);
    void destroyImageViews(Device device);

    const VkFormat getFormat() const;
    const VkExtent2D getExtents() const;
    const uint32_t getSwapChainSize() const;
    const VkImageView getSwapChainImageView(uint32_t i) const;

private:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};

#endif