#ifndef __COMMANDER_H_INCLUDED__
#define __COMMANDER_H_INCLUDED__

#include "util.h"


struct Device;

struct Commander {
    VkCommandPool pool;
    std::vector<VkCommandBuffer> buffers;

    void createPool(Instance* instance);
    void createBuffers(Instance* instance);

    void destroyPool(Device* device);
    void destroyBuffers(Device* device);

    void transitionImageLayout(Device* device, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
    void copyBuffer(Device* device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void copyBufferToImage(Device* device, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void generateMipmaps(Device* device, VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

private:
    VkCommandBuffer beginSingleTimeCommands(Device* device);
    void endSingleTimeCommands(Device* device, VkCommandBuffer commandBuffer);
};

#endif
