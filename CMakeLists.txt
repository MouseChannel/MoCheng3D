cmake_minimum_required(VERSION 3.0.0)
project(MoCheng3D VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR})

find_program(GLSLANG glslangValidator REQUIRED)
message(STATUS "run glslangValidator to compile shaders ...")
execute_process(COMMAND ${GLSLANG} -V ${CMAKE_SOURCE_DIR}/Shader/vert.vert -o ${CMAKE_SOURCE_DIR}/Shader/vert.spv)
execute_process(COMMAND ${GLSLANG} -V ${CMAKE_SOURCE_DIR}/Shader/frag.frag -o ${CMAKE_SOURCE_DIR}/Shader/frag.spv)
message(STATUS "compile shader OK")
 
include(cmake/FindGLFW.cmake)
include(cmake/FindVulkan.cmake)


file(GLOB_RECURSE SRC src   *.cpp  )

add_library(mocheng3d  ${SRC})

 
target_link_libraries(mocheng3d glfw Vulkan::Vulkan)
add_subdirectory(Example)

 
 