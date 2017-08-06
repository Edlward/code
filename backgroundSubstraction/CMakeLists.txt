cmake_minimum_required(VERSION 2.8)
project(background)
set(OpenCV_DIR "/home/lxg/app/opencv-2.4.9/build/")

find_package(OpenCV REQUIRED)
set(CMAKE_CXX_FLAGS -std=c++11)

add_executable(bs main.cpp camera.cpp backstraction.cpp)
target_link_libraries(bs ${OpenCV_LIBS})