#include "texture.h"
#include "commander.h"
#include "descriptor.h"
#include "device.h"
#include "include.h"
#include "instance.h"
#include "model.h"
#include "renderer.h"
#include "surface.h"
#include "sync.h"
#include "util.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void Texture::create(Instance* instance, std::string imgPath) {
  createTextureImage(instance, imgPath);
  createTextureImageView(instance->device);
  createTextureSampler(instance->device);
}

void Texture::destroy(Device* device) {
  vkDestroySampler(device->logical, sampler, nullptr);
  vkDestroyImageView(device->logical, view, nullptr);
  vkDestroyImage(device->logical, image, nullptr);
  vkFreeMemory(device->logical, memory, nullptr);
}

void Texture::createTextureImage(Instance* instance, std::string imgPath) {
  int texWidth, texHeight, texChannels;
  stbi_uc* pixels = stbi_load(imgPath.c_str(), &texWidth, &texHeight,
                              &texChannels, STBI_rgb_alpha);
  VkDeviceSize imageSize = texWidth * texHeight * 4;
  if (!pixels) {
	throw std::runtime_error("failed to load texture image!");
  }
  mipLevels = static_cast<uint32_t>(
                  std::floor(std::log2(std::max(texWidth, texHeight)))) +
              1;
  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  createBuffer(instance->device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               stagingBuffer, stagingBufferMemory);
  void* data;
  vkMapMemory(instance->device->logical, stagingBufferMemory, 0, imageSize, 0,
              &data);
  memcpy(data, pixels, static_cast<size_t>(imageSize));
  vkUnmapMemory(instance->device->logical, stagingBufferMemory);
  stbi_image_free(pixels);
  createImage(instance->device, texWidth, texHeight, mipLevels,
              VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB,
              VK_IMAGE_TILING_OPTIMAL,
              VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                  VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, memory);
  instance->commander->transitionImageLayout(
      instance->device, image, VK_FORMAT_R8G8B8A8_SRGB,
      VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      mipLevels);
  instance->commander->copyBufferToImage(instance->device, stagingBuffer, image,
                                         static_cast<uint32_t>(texWidth),
                                         static_cast<uint32_t>(texHeight));
  //transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels);
  instance->commander->generateMipmaps(instance->device, image,
                                       VK_FORMAT_R8G8B8A8_SRGB, texWidth,
                                       texHeight, mipLevels);
  vkDestroyBuffer(instance->device->logical, stagingBuffer, nullptr);
  vkFreeMemory(instance->device->logical, stagingBufferMemory, nullptr);
}

void Texture::createTextureImageView(Device* device) {
  view = createImageView(device, image, VK_FORMAT_R8G8B8A8_SRGB,
                         VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
}

void Texture::createTextureSampler(Device* device) {
  VkSamplerCreateInfo samplerInfo = {};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;
  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.anisotropyEnable = VK_TRUE;
  samplerInfo.maxAnisotropy = 16;
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = static_cast<float>(mipLevels);
  if (vkCreateSampler(device->logical, &samplerInfo, nullptr, &sampler) !=
      VK_SUCCESS) {
	throw std::runtime_error("failed to create texture sampler!");
  }
}
