#ifndef __DESCRIPTOR_H_INCLUDED__
#define __DESCRIPTOR_H_INCLUDED__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <array>

#include "util.h"


struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

class Descriptor {
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    void createDescriptorSetLayout(Device device);
    void createUniformBuffers(Device device, uint32_t swapChainSize);
    void createDescriptorPool(Device device, uint32_t swapChainSize);
    void createDescriptorSets(Device device, uint32_t swapChainSize);

public:
    void create(Instance instance);
};

#endif