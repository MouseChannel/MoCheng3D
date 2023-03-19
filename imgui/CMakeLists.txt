file(GLOB_RECURSE IMGUILIB ./ *.cpp)
set(IMGUIDIR "D:/MoCheng3D/imgui")
add_library(imguiLib ${IMGUILIB})
target_link_libraries(imguiLib glfw Vulkan)
 