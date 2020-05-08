#ifndef __INSTANCE_H_INCLUDED__
#define __INSTANCE_H_INCLUDED__

struct Device;
struct Surface;
struct Renderer;
struct Descriptor;
struct Commander;
struct Sync;
struct Model;

struct Instance {
	bool validationLayersEnabled;
	uint32_t currentFrame;
	bool framebufferResized;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	Device* device;
	Surface* surface;
	Renderer* renderer;
	Descriptor* descriptor;
	Commander* commander;
	Sync* sync;
	std::vector<Model> models;

	Instance();

	void create(bool enableValidationLayers);
	void destroy();
	bool shouldClose();
	void waitIdle();

	void drawFrame();

  private:
	void createInstance();
	void setupDebugMessenger();
	void createSurface();

	void destroyDebugMessenger();

	void cleanupSwapChain();
	void recreateSwapChain();
};

#endif