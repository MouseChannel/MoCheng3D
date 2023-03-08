need `GLFW` and `Vulkan` SDK

---

in `cmake/FindGLFW.cmake` set `GLFW_DIR` = GLFW_dir.

in  `cmake/FindVulkan.cmake` set `VULKAN_DIR` = Vulkan_dir

`cmake -GNinja ..`

`Ninja all`

`./build/Example/mytest.exe`
