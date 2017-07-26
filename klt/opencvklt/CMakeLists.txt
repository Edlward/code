cmake_minimum_required(VERSION 3.0)
project(opencvKLT)

find_package(OpenCV REQUIRED)

include_directories(..)
link_directories(..)

add_executable(opencvKLT main.cpp)
target_link_libraries(opencvKLT klt ${OpenCV_LIBS})