cmake_minimum_required(VERSION 3.5)
project(lab4)

set(CMAKE_CXX_STANDARD 14)
set(SFML_DIR "./SFML-2.5.1/lib/cmake/SFML")
set(SFML_INCLUDE_DIR "./SFML-2.5.1/include")
set(SFML_LIBRARY_DIR "./SFML-2.5.1/lib")

include_directories(${SFML_INCLUDE_DIR})
find_package(SFML COMPONENTS network system window graphics REQUIRED)

#add_executable(client tcpClient.cpp)
add_executable(server tcpServer.cpp)
#target_link_libraries(client sfml-network)
target_link_libraries(server sfml-network sfml-system)
