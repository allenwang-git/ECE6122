cmake_minimum_required(VERSION 3.10)
project(GaTech_Buzzy_Bowl)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenGL REQUIRED)

# compile glfw glew
add_subdirectory(third-party)

include_directories(
        third-party/glfw/include/GLFW/
        third-party/glew/include/
        include/
        common/
)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW
        )
file(GLOB sources "source/*")

add_executable(gtbb
        ${sources}
        common/shader.cpp
        common/texture.cpp
        common/objloader.cpp
        common/vboindexer.cpp
        common/controls.cpp

        )
target_link_libraries(gtbb ${ALL_LIBS})