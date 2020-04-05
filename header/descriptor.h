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

struct Descriptor {
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    uint32_t nIndices;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    void createDescriptorSetLayout(Instance instance);
    void createVertexBuffer(Instance instance, std::vector<Vertex> vertices);
    void createIndexBuffer(Instance instance, std::vector<uint32_t> indices);
    void createUniformBuffers(Instance instance);
    void createDescriptorPool(Instance instance);
    void createDescriptorSets(Instance instance);
};

#endif