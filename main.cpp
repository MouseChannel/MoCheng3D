/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-03-03 16:47:14
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-03-03 16:47:24
 * @FilePath: \MYP\main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
 
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
