cmake_minimum_required(VERSION 2.8)
project(myapp)

# set(OpenCV_DIR "/home/lxg/app/opencv-2.4.9/debug/")

add_definitions(-std=c++11) 
find_package(OpenCV 3.3.0 REQUIRED)

add_executable(test1 singleObject.cpp)
target_link_libraries(test1 ${OpenCV_LIBS})