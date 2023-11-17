#pragma once

#include "device.hpp"
#include <cstdlib>
#define GLFW_INCLUDE_VULKAN

#include <iostream>

#include "pipeline.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

#include <memory>
#include <vector>
#include <stdexcept>

class Runner {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;
  void run(){};

  Runner() {
      createPipelineLayout();
      createPipeline();
      createCommandBuffers();
  }
  ~Runner() {
      vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
  }

  Runner(const Runner&) = delete;
  Runner& operator=(const Runner&) = delete;

private:
    void createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::exception("Failed to create pipeline layout.");
    }
    void createPipeline() {
        auto pipelineConfig = engine::Pipeline::defaultPipelineConfiguration(swapChain.width(), swapChain.height());
        pipelineConfig.renderPass = swapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<engine::Pipeline>(
            "C:/Users/mague/Source/Repos/Engine/Engine/Shaders/simple_shader.vert.svt",
            "C:/Users/mague/Source/Repos/Engine/Engine/Shaders/simple_shader.frag.svt",
            device,
            pipelineConfig
        );
    }
    void createCommandBuffers() {
    
    }
    void drawFrame() {

    }

  engine::Window window{WIDTH, HEIGHT, "SomeName"};
  engine::Device device{window};
  engine::SwapChain swapChain{ device, window.getExtent() };
  /*
  engine::Pipeline Pipeline{
      "C:/Users/mague/Source/Repos/Engine/Engine/Shaders/simple_shader.vert.svt",
      "C:/Users/mague/Source/Repos/Engine/Engine/Shaders/simple_shader.frag.svt",
      device,
      engine::Pipeline::defaultPipelineConfiguration(WIDTH, HEIGHT)};
  */
  std::unique_ptr<engine::Pipeline> pipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
};

int main() {
  Runner app{};

  try {
    app.run();
  } catch (const std::exception &ex) {
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
