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
    VkImage textureImage;
    VkDeviceMemory textureMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

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

    void createDescriptorSetLayout(Device device);
    void createVertexBuffer(Instance instance, std::vector<Vertex> vertices);
    void createIndexBuffer(Instance instance, std::vector<uint32_t> indices);
    void createUniformBuffers(Device device, uint32_t swapChainSize);
    void createDescriptorPool(Device device, uint32_t swapChainSize);
    void createDescriptorSets(Device device, uint32_t swapChainSize);


    void createTextureImage();
    void createTextureImageView();
    void createTextureSampler();

public:
    void create(Instance instance);

    const VkBuffer getVertexBuffer() const;
    const VkBuffer getIndexBuffer() const;
    const uint32_t getNIndices() const;
    const VkDescriptorSet* getDescriptorSets(uint32_t i) const;
};

#endif