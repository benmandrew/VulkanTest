#include "commander.h"
#include "descriptor.h"
#include "device.h"
#include "include.h"
#include "instance.h"
#include "model.h"
#include "renderer.h"
#include "surface.h"
#include "sync.h"
#include "texture.h"
#include "util.h"


//#define NDEBUG
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif


void run(Instance* instance) {
    while(!instance->shouldClose()) {
        glfwPollEvents();
        instance->drawFrame();
    }
    instance->waitIdle();
}

int main() {
    Instance instance = Instance();
    instance.create(enableValidationLayers);
    std::cout << "Instance created" << std::endl;
    try {
        run(&instance);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
