#ifndef __INSTANCE_H_INCLUDED__
#define __INSTANCE_H_INCLUDED__

#include "commander.h"
#include "descriptor.h"
#include "model.h"
#include "renderer.h"
#include "surface.h"
#include "sync.h"

struct Device {
    VkDevice logical;
    VkPhysicalDevice physical = VK_NULL_HANDLE;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    void pickPhysicalDevice(Instance* instance);
    void createLogicalDevice(Instance* instance, bool enableValidationLayers);

    void destroyLogicalDevice();

private:
    bool isDeviceSuitable(Instance* instance, VkPhysicalDevice device);
};

struct Instance {
    bool validationLayersEnabled;
    uint32_t currentFrame;
    bool framebufferResized;
    VkInstance instance;
    VkDebugUtilsMessengerEXT* debugMessenger;
    Device* device;
    Surface* surface;
    Renderer* renderer;
    Descriptor* descriptor;
    Commander* commander;
    Sync* sync;
    std::vector<Model> models;

    void create(bool enableValidationLayers);
    void destroy();
    bool shouldClose();
    void waitIdle();

    void drawFrame();

private:
    void createInstance();
    void setupDebugMessenger();
    void createSurface();

    void destroyDebugMessenger();

    void cleanupSwapChain();
    void recreateSwapChain();
};

#endif