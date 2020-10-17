#include "renderer.h"
#include "commander.h"
#include "descriptor.h"
#include "device.h"
#include "include.h"
#include "instance.h"
#include "model.h"
#include "surface.h"
#include "sync.h"
#include "texture.h"
#include "util.h"

void Renderer::createRenderPass(Instance* instance) {
	msaaSamples = getMaxUsableSampleCount(instance->device);
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = instance->surface->getFormat();
	colorAttachment.samples = msaaSamples;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = findDepthFormat(instance->device);
	depthAttachment.samples = msaaSamples;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout =
	    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	VkAttachmentDescription colourAttachmentResolve = {};
	colourAttachmentResolve.format = instance->surface->getFormat();
	colourAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
	colourAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colourAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colourAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colourAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colourAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colourAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout =
	    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	VkAttachmentReference colourAttachmentResolveRef = {};
	colourAttachmentResolveRef.attachment = 2;
	colourAttachmentResolveRef.layout =
	    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	subpass.pResolveAttachments = &colourAttachmentResolveRef;
	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
	                           VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	std::array<VkAttachmentDescription, 3> attachments = {
	    colorAttachment, depthAttachment, colourAttachmentResolve};
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;
	if (vkCreateRenderPass(instance->device->logical, &renderPassInfo, nullptr,
	                       &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
}

void Renderer::createGraphicsPipeline(Instance* instance) {
	auto vertShaderCode = readFile("shaders/shader.vert.spv");
	auto fragShaderCode = readFile("shaders/shader.frag.spv");
	VkShaderModule vertShaderModule =
	    createShaderModule(instance->device, vertShaderCode);
	VkShaderModule fragShaderModule =
	    createShaderModule(instance->device, fragShaderCode);
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";
	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";
	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
	                                                  fragShaderStageInfo};
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount =
	    static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	const VkExtent2D swapChainExtent = instance->surface->getExtents();
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f;          // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f;    // Optional
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = msaaSamples;
	multisampling.minSampleShading = 1.0f;          // Optional
	multisampling.pSampleMask = nullptr;            // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE;      // Optional
	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {};  // Optional
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask =
	    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
	    VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	/*colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;*/
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pSetLayouts = &instance->descriptor->descriptorSetLayout;
	pipelineLayoutInfo.setLayoutCount = 1;
	VkPushConstantRange pushConstantRange = {};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(UniformBufferObject);
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	if (vkCreatePipelineLayout(instance->device->logical, &pipelineLayoutInfo,
	                           nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1;              // Optional
	if (vkCreateGraphicsPipelines(instance->device->logical, VK_NULL_HANDLE, 1,
	                              &pipelineInfo, nullptr,
	                              &graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}
	vkDestroyShaderModule(instance->device->logical, fragShaderModule, nullptr);
	vkDestroyShaderModule(instance->device->logical, vertShaderModule, nullptr);
}

void Renderer::createFramebuffers(Instance* instance) {
	uint32_t swapChainSize = instance->surface->getSwapChainSize();
	swapChainFramebuffers.resize(swapChainSize);
	for (size_t i = 0; i < swapChainSize; i++) {
		std::array<VkImageView, 3> attachments = {
		    colourImageView, depthImageView,
		    instance->surface->swapChainImageViews[i]};
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount =
		    static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		const VkExtent2D swapChainExtent = instance->surface->getExtents();
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;
		if (vkCreateFramebuffer(instance->device->logical, &framebufferInfo,
		                        nullptr,
		                        &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void Renderer::createColourResources(Instance* instance) {
	VkFormat colourFormat = instance->surface->getFormat();
	const VkExtent2D swapChainExtent = instance->surface->getExtents();
	createImage(instance->device, swapChainExtent.width, swapChainExtent.height,
	            1, msaaSamples, colourFormat, VK_IMAGE_TILING_OPTIMAL,
	            VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |
	                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colourImage,
	            colourImageMemory);
	colourImageView =
	    createImageView(instance->device, colourImage, colourFormat,
	                    VK_IMAGE_ASPECT_COLOR_BIT, 1);
}

void Renderer::createDepthResources(Instance* instance) {
	VkFormat depthFormat = findDepthFormat(instance->device);
	const VkExtent2D swapChainExtent = instance->surface->getExtents();
	const uint32_t mipLevels =
	    std::floor(std::log2(
	        std::max(swapChainExtent.width, swapChainExtent.height))) +
	    1;
	createImage(instance->device, swapChainExtent.width, swapChainExtent.height,
	            1, msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL,
	            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
	            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage,
	            depthImageMemory);
	depthImageView = createImageView(instance->device, depthImage, depthFormat,
	                                 VK_IMAGE_ASPECT_DEPTH_BIT, 1);
	instance->commander->transitionImageLayout(
	    instance->device, depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED,
	    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
}

VkSampleCountFlagBits Renderer::getMaxUsableSampleCount(Device* device) {
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(device->physical, &physicalDeviceProperties);
	VkSampleCountFlags counts =
	    physicalDeviceProperties.limits.framebufferColorSampleCounts &
	    physicalDeviceProperties.limits.framebufferDepthSampleCounts;
	if (counts & VK_SAMPLE_COUNT_64_BIT) {
		return VK_SAMPLE_COUNT_64_BIT;
	}
	if (counts & VK_SAMPLE_COUNT_32_BIT) {
		return VK_SAMPLE_COUNT_32_BIT;
	}
	if (counts & VK_SAMPLE_COUNT_16_BIT) {
		return VK_SAMPLE_COUNT_16_BIT;
	}
	if (counts & VK_SAMPLE_COUNT_8_BIT) {
		return VK_SAMPLE_COUNT_8_BIT;
	}
	if (counts & VK_SAMPLE_COUNT_4_BIT) {
		return VK_SAMPLE_COUNT_4_BIT;
	}
	if (counts & VK_SAMPLE_COUNT_2_BIT) {
		return VK_SAMPLE_COUNT_2_BIT;
	}
	return VK_SAMPLE_COUNT_1_BIT;
}

void Renderer::destroyRenderPass(Device* device) {
	vkDestroyRenderPass(device->logical, renderPass, nullptr);
}

void Renderer::destroyGraphicsPipeline(Device* device) {
	vkDestroyPipeline(device->logical, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device->logical, pipelineLayout, nullptr);
}

void Renderer::destroyFramebuffers(Instance* instance) {
	uint32_t swapChainSize = instance->surface->getSwapChainSize();
	for (size_t i = 0; i < swapChainSize; i++) {
		vkDestroyFramebuffer(instance->device->logical,
		                     swapChainFramebuffers[i], nullptr);
	}
}

void Renderer::destroyColourResources(Device* device) {
	vkDestroyImageView(device->logical, colourImageView, nullptr);
	vkDestroyImage(device->logical, colourImage, nullptr);
	vkFreeMemory(device->logical, colourImageMemory, nullptr);
}

void Renderer::destroyDepthResources(Device* device) {
	vkDestroyImageView(device->logical, depthImageView, nullptr);
	vkDestroyImage(device->logical, depthImage, nullptr);
	vkFreeMemory(device->logical, depthImageMemory, nullptr);
}

const VkRenderPassBeginInfo Renderer::getRenderPassInfo(
        Instance* instance, uint32_t frameIndex) const {
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapChainFramebuffers[frameIndex];
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = instance->surface->getExtents();
	return renderPassInfo;
}

const VkPipeline Renderer::getPipeline() const { return graphicsPipeline; }

const VkPipelineLayout Renderer::getPipelineLayout() const {
	return pipelineLayout;
}
