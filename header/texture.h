#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__

#include "util.h"

struct Texture {
	VkImage image;
	VkDeviceMemory memory;
	VkImageView view;
	VkSampler sampler;
	uint32_t mipLevels;

	void create(Instance* instance, std::string imgPath);
	void destroy(Device* device);

  private:
	void createTextureImage(Instance* instance, std::string imgPath);
	void createTextureImageView(Device* device);
	void createTextureSampler(Device* device);
};

#endif