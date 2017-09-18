cmake_minimum_required(VERSION 2.8)
project(tool)

set(CMAKE_CXX_FLAGS -std=c++11)

set(OpenCV_DIR "/home/lxg/app/opencv-3.3.0/build")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(crop crop.cpp)
target_link_libraries(crop ${OpenCV_LIBS})

add_executable(sample sample.cpp)
target_link_libraries(sample ${OpenCV_LIBS})

add_executable(tensorflow tensorflow.cpp)
target_link_libraries(tensorflow ${OpenCV_LIBS})