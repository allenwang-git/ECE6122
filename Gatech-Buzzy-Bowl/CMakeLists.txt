cmake_minimum_required(VERSION 3.10)
project(GaTech_Buzzy_Bowl)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenGL REQUIRED)

# compile glfw glew
add_subdirectory(third-party)

include_directories(
        third-party/glfw/include/GLFW/
        third-party/glew/include/GL/
        include/
)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW
        )

add_executable(gtbb main.cpp)
target_link_libraries(gtbb ${ALL_LIBS})
