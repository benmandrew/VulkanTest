#ifndef __COMMANDER_H_INCLUDED__
#define __COMMANDER_H_INCLUDED__

#include <stdexcept>
#include <vector>

#include "instance.h"
#include "util.h"

struct Device;

class Commander {
    VkCommandPool pool;
    std::vector<VkCommandBuffer> buffers;

    void createBuffers(Instance instance);

    VkCommandBuffer beginSingleTimeCommands(Device device);
    void endSingleTimeCommands(Device device, VkCommandBuffer commandBuffer);

public:
    void create();
    void destroy(Device device);

    void transitionImageLayout(Device device, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
    void copyBufferToImage(Device device, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void generateMipmaps(Device device, VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
};

#endif
