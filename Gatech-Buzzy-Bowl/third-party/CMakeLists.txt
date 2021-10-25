include_directories(
        glfw/include/GLFW/
        glew/include/)
## GLFW
add_subdirectory(glfw)
set(OPENGL_LIBRARY
        ${OPENGL_LIBRARY} -lGL -lGLU -lXrandr -lXext -lX11 -lrt
        ${CMAKE_DL_LIBRS}
        ${GLFW_LIBRARIES}
        )

## GLEW
set(GLEW_SOURCE glew/src/glew.c)
add_library(GLEW STATIC
        ${GLEW_SOURCE}
        ${GLEW_INCLUDE})
target_link_libraries(GLEW ${OPENGL_LIBRARY})
