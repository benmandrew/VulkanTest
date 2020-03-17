
VULKAN_SDK_PATH = /home/ben/dev/vulkan/1.2.131.2/x86_64
STB_INCLUDE_PATH = /home/ben/dev/stb

CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include -I$(STB_INCLUDE_PATH)
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

VulkanTest: main.cpp
	./shaders/compile.sh
	g++ $(CFLAGS) -o build/VulkanTest main.cpp $(LDFLAGS) -lpthread

.PHONY: test clean

test: VulkanTest
	LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/vulkan/explicit_layer.d ./build/VulkanTest

clean:
	rm -f VulkanTest

