#include "texture.h"


void Texture::create(Instance instance, std::string imgPath) {
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(imgPath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;
    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }
    mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(instance.device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    void* data;
    vkMapMemory(instance.device.logical, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(instance.device.logical, stagingBufferMemory);
    stbi_image_free(pixels);
    createImage(instance.device, texWidth, texHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, memory);
    instance.commander->transitionImageLayout(instance.device, image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
    instance.commander->copyBufferToImage(instance.device, stagingBuffer, image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    //transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels);
    instance.commander->generateMipmaps(instance.device, image, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);
    vkDestroyBuffer(instance.device.logical, stagingBuffer, nullptr);
    vkFreeMemory(instance.device.logical, stagingBufferMemory, nullptr);
}

void Texture::destroy(Device device) {
    vkDestroySampler(device.logical, sampler, nullptr);
    vkDestroyImageView(device.logical, view, nullptr);
    vkDestroyImage(device.logical, image, nullptr);
    vkFreeMemory(device.logical, memory, nullptr);
}