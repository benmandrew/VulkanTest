#ifndef __SYNC_H_INCLUDED__
#define __SYNC_H_INCLUDED__

#include "util.h"

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

struct Sync {
  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  std::vector<VkFence> imagesInFlight;

  void createSyncObjects(Instance* instance);
  void destroySyncObjects(Device* device);
};

#endif