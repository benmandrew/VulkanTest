#ifndef __DEVICE_H_INCLUDED__
#define __DEVICE_H_INCLUDED__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct Device {
    VkDevice logical;
    VkPhysicalDevice physical;
    VkQueue graphicsQueue;
    VkQueue presentQueue;


};

#endif