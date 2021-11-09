cmake_minimum_required(VERSION 3.10)
project(GaTech_Buzzy_Bowl)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenGL REQUIRED)
#include(CreateLaunchers)
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


        )
target_link_libraries(gtbb ${ALL_LIBS})
#set_target_properties(gtbb PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source/")
#create_target_launcher(gtbb WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/source")
#
#add_custom_command(
#        TARGET gtbb POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/source/"
#)