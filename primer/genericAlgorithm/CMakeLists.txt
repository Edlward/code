cmake_minimum_required(VERSION 2.8)
project(algorithm)

# add_definitions(-std=c++11)
set(CMAKE_CXX_FLAGS -std=c++11)

add_executable(lamda lambda.cpp)
add_executable(iterator iterator.cpp)

