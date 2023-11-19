#pragma once
#define GLFW_INCLUDE_VULKAN

#include "model.hpp"
#include "window.hpp"
#include "device.hpp"
#include "swap_chain.hpp"
#include "pipeline.hpp"

#include "pipeline.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <stdexcept>
#include <array>

namespace engine {
	class Runner{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		Runner();
		~Runner();

		Runner(const Runner&) = delete;
		Runner& operator=(const Runner&) = delete;

		void run();

	private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void freeCommandBuffers();
		void wasWindowResized();

		Window window{ WIDTH, HEIGHT, "Hello Vulkan!" };
		Device device{ window };
		std::unique_ptr <SwapChain> swapChain;
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<Model> model;
	};
}