cmake_minimum_required(VERSION 2.8)
project(cluster)

find_package(OpenCV REQUIRED)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(cluster main.cpp cluster.cpp)
target_link_libraries(cluster ${OpenCV_LIBS})