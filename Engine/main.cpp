#pragma once

#include "runner.hpp"
#include <cstdlib>


#include <iostream>

int main() {
  engine::Runner app{};

  try {
    app.run();
  } catch (const std::exception &ex) {
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
