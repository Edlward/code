cmake_minimum_required(VERSION 2.8)
project(xmltest)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

add_definitions(-std=c++11)
add_executable(xmltest main.cpp tinyxml2.cpp)