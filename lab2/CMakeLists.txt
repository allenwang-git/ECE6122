cmake_minimum_required(VERSION 3.10)
project(lab2)

set(CMAKE_CXX_STANDARD 14)

add_executable(lab2_1 Lab2_Problem1.cpp)
target_link_libraries(lab2_1 pthread)
add_executable(lab2_2 Lab2_Problem2.cpp)