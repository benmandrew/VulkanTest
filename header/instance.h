#ifndef __INSTANCE_H_INCLUDED__
#define __INSTANCE_H_INCLUDED__

#include "surface.h"


class Commander;

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct Device {
    VkDevice logical;
    VkPhysicalDevice physical = VK_NULL_HANDLE;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
};

struct Instance {
    bool validationLayersEnabled;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    Device device;
    Surface surface;
    Commander* commander;

    void create(bool enableValidationLayers);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

private:
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
};

#endif