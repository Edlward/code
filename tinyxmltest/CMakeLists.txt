cmake_minimum_required(VERSION 2.8)
project(xmltest)
add_definitions(-std=c++11)
add_executable(xmltest main.cpp tinyxml2.cpp)