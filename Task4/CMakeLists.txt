cmake_minimum_required(VERSION 3.6)
project(Task4)

set(CMAKE_CXX_STANDARD 11)

add_compile_options("-O3")

set(SOURCE_FILES main.cpp)
add_executable(Task4 ${SOURCE_FILES})