# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(ConnectFourGUI)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(connect4 main.cpp)
target_link_libraries(connect4 PRIVATE sfml-graphics sfml-window sfml-system)
