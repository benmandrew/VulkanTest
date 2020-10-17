#ifndef __DESCRIPTOR_H_INCLUDED__
#define __DESCRIPTOR_H_INCLUDED__

#include "util.h"

struct Vertex;

struct UniformBufferObject {
	// alignas(16) glm::mat4 model;
	// alignas(16) glm::mat4 view;
	// alignas(16) glm::mat4 proj;
    alignas(16) glm::mat4 mvp;
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

    UniformBufferObject ubo;

	void createDescriptorSetLayout(Instance* instance);
	void createVertexBuffer(Instance* instance, std::vector<Vertex> vertices);
	void createIndexBuffer(Instance* instance, std::vector<uint32_t> indices);
	void createUniformBuffers(Instance* instance);
	void createDescriptorPool(Instance* instance);
	void createDescriptorSets(Instance* instance);

	void destroyDescriptorSetLayout(Device* device);
	void destroyVertexBuffer(Device* device);
	void destroyIndexBuffer(Device* device);
	void destroyUniformBuffers(Instance* instance);
	void destroyDescriptorPool(Device* device);

	void updateUniformBuffer(Instance* instance, uint32_t currentImage);
};

#endif