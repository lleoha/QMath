project(QFloatTests)
cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)
include_directories(AFTER ./include)

enable_testing()
add_subdirectory(tests)