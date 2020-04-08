#include "commander.h"
#include "descriptor.h"
#include "device.h"
#include "include.h"
#include "instance.h"
#include "model.h"
#include "renderer.h"
#include "surface.h"
#include "sync.h"
#include "texture.h"
#include "util.h"


void Descriptor::createDescriptorSetLayout(Instance* instance) {
    uint32_t swapChainSize = instance->surface->getSwapChainSize();
    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();
    if (vkCreateDescriptorSetLayout(instance->device->logical, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void Descriptor::createVertexBuffer(Instance* instance, std::vector<Vertex> vertices) {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(instance->device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    void* data;
    vkMapMemory(instance->device->logical, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(instance->device->logical, stagingBufferMemory);
    createBuffer(instance->device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
    instance->commander->copyBuffer(instance->device, stagingBuffer, vertexBuffer, bufferSize);
    vkDestroyBuffer(instance->device->logical, stagingBuffer, nullptr);
    vkFreeMemory(instance->device->logical, stagingBufferMemory, nullptr);
}

void Descriptor::createIndexBuffer(Instance* instance, std::vector<uint32_t> indices) {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(instance->device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    void* data;
    vkMapMemory(instance->device->logical, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(instance->device->logical, stagingBufferMemory);
    createBuffer(instance->device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);
    instance->commander->copyBuffer(instance->device, stagingBuffer, indexBuffer, bufferSize);
    vkDestroyBuffer(instance->device->logical, stagingBuffer, nullptr);
    vkFreeMemory(instance->device->logical, stagingBufferMemory, nullptr);
}

void Descriptor::createUniformBuffers(Instance* instance) {
    uint32_t swapChainSize = instance->surface->getSwapChainSize();
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    uniformBuffers.resize(swapChainSize);
    uniformBuffersMemory.resize(swapChainSize);
    for (size_t i = 0; i < swapChainSize; i++) {
        createBuffer(instance->device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
    }
}

void Descriptor::createDescriptorPool(Instance* instance) {
    uint32_t swapChainSize = instance->surface->getSwapChainSize();
    std::array<VkDescriptorPoolSize, 2> poolSizes = {};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(swapChainSize);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(swapChainSize);
    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(swapChainSize);
    if (vkCreateDescriptorPool(instance->device->logical, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void Descriptor::createDescriptorSets(Instance* instance) {
    uint32_t swapChainSize = instance->surface->getSwapChainSize();
    std::vector<VkDescriptorSetLayout> layouts(swapChainSize, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainSize);
    allocInfo.pSetLayouts = layouts.data();
    descriptorSets.resize(swapChainSize);
    if (vkAllocateDescriptorSets(instance->device->logical, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
    for (size_t i = 0; i < swapChainSize; i++) {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);
        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = instance->models[0].texture->view;
        imageInfo.sampler = instance->models[0].texture->sampler;
        std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};
        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;
        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;
        vkUpdateDescriptorSets(instance->device->logical, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void Descriptor::destroyDescriptorSetLayout(Device* device) {
    vkDestroyDescriptorSetLayout(device->logical, descriptorSetLayout, nullptr);
}

void Descriptor::destroyVertexBuffer(Device* device) {
    vkDestroyBuffer(device->logical, vertexBuffer, nullptr);
    vkFreeMemory(device->logical, vertexBufferMemory, nullptr);
}

void Descriptor::destroyIndexBuffer(Device* device) {
    vkDestroyBuffer(device->logical, indexBuffer, nullptr);
    vkFreeMemory(device->logical, indexBufferMemory, nullptr);
}

void Descriptor::destroyUniformBuffers(Instance* instance) {
    uint32_t swapChainSize = instance->surface->getSwapChainSize();
    for (size_t i = 0; i < swapChainSize; i++) {
        vkDestroyBuffer(instance->device->logical, uniformBuffers[i], nullptr);
        vkFreeMemory(instance->device->logical, uniformBuffersMemory[i], nullptr);
    }
}

void Descriptor::destroyDescriptorPool(Device* device) {
    vkDestroyDescriptorPool(device->logical, descriptorPool, nullptr);
}

void Descriptor::destroyDescriptorSets(Device* device) {

}

void Descriptor::updateUniformBuffer(Instance* instance, uint32_t currentImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    const VkExtent2D swapChainExtent = instance->surface->getExtents();
    UniformBufferObject ubo = {};
    ubo.model = glm::rotate(glm::mat4(1.0f), 0.1f * time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;
    void* data;
    vkMapMemory(instance->device->logical, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(instance->device->logical, uniformBuffersMemory[currentImage]);
}
