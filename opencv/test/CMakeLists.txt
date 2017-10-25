cmake_minimum_required(VERSION 2.8)
project(test)

set(OpenCV_DIR "/home/lxg/app/opencv-3.3.0/build/")

find_package(OpenCV)
message(STATUS ${OpenCV_LIBS})
message(STATUS ${OpenCV_INCS})


add_executable(read main.cpp)
target_link_libraries(read ${OpenCV_LIBS})