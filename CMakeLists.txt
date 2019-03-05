cmake_minimum_required(VERSION 3.13)
project(braille)

find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 14)

add_executable(braille src/main.cpp src/CLI11.hpp)
target_link_libraries(braille ${OpenCV_LIBS})
