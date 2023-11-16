#pragma once

#include "device.hpp"
#include <cstdlib>
#define GLFW_INCLUDE_VULKAN

// #include <GL/gl.h>
// #include <GLFW/glfw3.h>

// #define GLM_FORCE_RADIANS
// #define GLM_FORCE_DEPTH_ZERO_TO_ONE
// #include <glm/vec4.hpp>
// #include <glm/mat4x4.hpp>

#include <iostream>

#include "pipeline.hpp"
#include "window.hpp"

class Runner {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;
  void run(){};

private:
  engine::Window Window{WIDTH, HEIGHT, "SomeName"};
  engine::ProtocolDevice device{Window};
  engine::Pipeline Pipeline{
      "C:/Users/mague/Source/Repos/Engine/Engine/Shaders/simple_shader.vert.svt",
      "C:/Users/mague/Source/Repos/Engine/Engine/Shaders/simple_shader.frag.svt",
      device,
      engine::Pipeline::defaultPipelineConfiguration(WIDTH, HEIGHT)};
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
