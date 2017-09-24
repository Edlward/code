cmake_minimum_required(VERSION 2.8)
project(head)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

set(CMAKE_CXX_FLAGS -std=c++11)
find_package(OpenCV)
add_executable(mask headMask.cpp)
target_link_libraries(mask ${OpenCV_LIBS})