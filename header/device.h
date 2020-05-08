#ifndef __DEVICE_H_INCLUDED__
#define __DEVICE_H_INCLUDED__

#include "util.h"

struct Device {
	VkDevice logical;
	VkPhysicalDevice physical = VK_NULL_HANDLE;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	void pickPhysicalDevice(Instance* instance);
	void createLogicalDevice(Instance* instance, bool enableValidationLayers);

	void destroyLogicalDevice();

  private:
	bool isDeviceSuitable(Instance* instance, VkPhysicalDevice device);
};

#endif