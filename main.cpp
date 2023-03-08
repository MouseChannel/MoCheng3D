 
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
// #include "vulkan/vulkan.hpp"
#include <iostream>
#include <vector>

int main(int, char **) {
  std::vector<int> aa;
  aa.push_back(1);
  aa.push_back(23);

  glfwInit();
  auto window = glfwCreateWindow(800, 800, "null", nullptr, nullptr);
  while (true) {
    glfwPollEvents();
  }
  std::cout << "Hello, world!\n";
}
