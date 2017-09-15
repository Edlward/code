cmake_minimum_required(VERSION 2.8)
project(crop)

set(CMAKE_CXX_FLAGS -std=c++11)
set(OpenCV_DIR "/home/lxg/app/opencv-3.3.0/build")
find_package(OpenCV REQUIRED)
add_executable(crop crop.cpp)
target_link_libraries(crop ${OpenCV_LIBS})