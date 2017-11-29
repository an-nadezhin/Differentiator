cmake_minimum_required(VERSION 3.8)
project(differentiator)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES differentiator.cpp differentiator.h)
add_executable(differentiator ${SOURCE_FILES})