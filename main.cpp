
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
// #include "SDL.h"
#include <iostream>
#include <vector>

int main(int, char**)
{
    std::vector<int> aa;
    aa.push_back(1);
    aa.push_back(23);

    // SDL_Init(SDL_INIT_EVERYTHING);
    // SDL_CreateWindow("21", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);

    //   glfwInit();
    //   auto window = glfwCreateWindow(800, 800, "null", nullptr, nullptr);
    //   while (true) {
    //     glfwPollEvents();
    //   }
    std::cout << "Hello, world!\n";
}
