cmake_minimum_required(VERSION 3.14.5)

project(thread_test)
set(CMAKE_CXX_STANDARD 14)
include_directories(dependencies/thunderclap/include)
include_directories(dependencies/parallel_lines/include)
add_executable(main
main.cpp
)