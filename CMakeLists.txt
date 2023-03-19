cmake_minimum_required(VERSION 3.0.0)
project(MoCheng3D)

set(CMAKE_CXX_STANDARD 20)

find_program(GLSLANG glslangValidator REQUIRED)
message(STATUS "run glslangValidator to compile shaders ...")
execute_process(COMMAND ${GLSLANG} -V ${CMAKE_SOURCE_DIR}/Shader/vert.vert -o ${CMAKE_SOURCE_DIR}/Shader/vert.spv)
execute_process(COMMAND ${GLSLANG} -V ${CMAKE_SOURCE_DIR}/Shader/frag.frag -o ${CMAKE_SOURCE_DIR}/Shader/frag.spv)
message(STATUS "compile shader OK")

include(cmake/FindGLFW.cmake)

include(cmake/FindVulkan.cmake)
include_directories(${PROJECT_SOURCE_DIR})

# imgui
file(GLOB_RECURSE IMGUI ./imgui *.cpp) 
add_library(imgui ${IMGUI})
target_link_libraries(imgui glfw Vulkan)



# source
file(GLOB_RECURSE SRC src *.cpp)
add_library(mocheng3d ${SRC})

target_link_libraries(mocheng3d imgui)
add_subdirectory(Example)
