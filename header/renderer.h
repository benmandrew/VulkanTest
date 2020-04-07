#ifndef __RENDERER_H_INCLUDED__
#define __RENDERER_H_INCLUDED__

#include "util.h"


struct Vertex {
    glm::vec3 pos;
    glm::vec3 colour;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, colour);
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && colour == other.colour && texCoord == other.texCoord;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                   (hash<glm::vec3>()(vertex.colour) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

struct Renderer {
    VkSampleCountFlagBits msaaSamples;
    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    VkImage colourImage;
    VkDeviceMemory colourImageMemory;
    VkImageView colourImageView;

    void createRenderPass(Instance* instance);
    void createGraphicsPipeline(Instance* instance);
    void createFramebuffers(Instance* instance);
    void createColourResources(Instance* instance);
    void createDepthResources(Instance* instance);
    VkSampleCountFlagBits getMaxUsableSampleCount(Device* device);

    void destroyRenderPass(Device* device);
    void destroyGraphicsPipeline(Device* device);
    void destroyFramebuffers(Instance* instance);
    void destroyColourResources(Device* device);
    void destroyDepthResources(Device* device);

    const VkRenderPassBeginInfo getRenderPassInfo(Instance* instance, uint32_t frameIndex) const;
    const VkPipeline getPipeline() const;
    const VkPipelineLayout getPipelineLayout() const;
};


#endif