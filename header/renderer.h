#ifndef __RENDERER_H_INCLUDED__
#define __RENDERER_H_INCLUDED__

#include "util.h"

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

  const VkRenderPassBeginInfo getRenderPassInfo(Instance* instance,
                                                uint32_t frameIndex) const;
  const VkPipeline getPipeline() const;
  const VkPipelineLayout getPipelineLayout() const;
};

#endif