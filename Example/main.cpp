#include "Example/app.cpp"
#include "MoCheng3D/Rendering/GLFW_Window.hpp"
#include "MoCheng3D/Wrapper/Device.hpp"
#include "MoCheng3D/Wrapper/Instance.hpp"
#include <iostream>
int main(int, char **) {

  std::cout << "Hello, world!\n";
  
  MoCheng3D::App a;
  a.Init();
  a.Run();
  
  
}
