cmake_minimum_required(VERSION 3.5)
project(lab4)

set(CMAKE_CXX_STANDARD 11)
#set(SFML_DIR "./SFML-2.5.1/lib/cmake/SFML")
set(SFML_INCLUDE_DIR "./SFML-2.5.1/include")
set(SFML_LIBRARY_DIR "./SFML-2.5.1/lib")
include_directories(${SFML_INCLUDE_DIR})
link_directories(./SFML-2.5.1/lib)
#find_package(SFML COMPONENTS network system REQUIRED)

add_executable(client tcpClient.cpp)
target_link_libraries(client sfml-network sfml-system)

add_executable(server tcpServer.cpp)
target_link_libraries(server sfml-network sfml-system)