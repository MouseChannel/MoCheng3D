IF(WIN32)
    MESSAGE(STATUS "Find Windows glfw😊")
    set(GLFW_DIR "D:/Some_software/glfw" CACHE PATH "123")
    set(GLWF_INCLUDE_DIR "${GLFW_DIR}/include")
    set(GLFW_LIB_LOCATION "${GLFW_DIR}/build/src/libglfw3.a")
    add_library(glfw STATIC IMPORTED GLOBAL)
    set_target_properties(
        glfw
        PROPERTIES
        IMPORTED_LOCATION ${GLFW_LIB_LOCATION}
        INTERFACE_INCLUDE_DIRECTORIES ${GLWF_INCLUDE_DIR}
    )
    MESSAGE(STATUS "link Windows glfw down😊")
ELSEIF(UNIX)
    MESSAGE(STATUS "Find linux glfw........")
    find_package(glfw REQUIRED)
ENDIF()