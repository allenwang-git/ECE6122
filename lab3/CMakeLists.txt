cmake_minimum_required(VERSION 3.5)
project(lab3)

set(CMAKE_CXX_STANDARD 11)

set(SFML_DIR "./SFML-2.5.1/lib/cmake/SFML")
set(SFML_INCLUDE_DIR "./SFML-2.5.1/include")
set(SFML_LIBRARY_DIR "./SFML-2.5.1/lib")
#set(DCMAKE_CXX_COMPILER $ (which g++))
#set(DCMAKE_C_COMPILER $ (which gcc))
include_directories(fonts)
include_directories(graphics)
include_directories(sound)
include_directories(${SFML_INCLUDE_DIR})
include_directories(code/include)

file(GLOB sources "code/*.cpp")
find_package(SFML COMPONENTS graphics audio window system REQUIRED)
add_executable(lab3 ${sources})
target_link_libraries(lab3 sfml-audio sfml-graphics sfml-window sfml-system)

