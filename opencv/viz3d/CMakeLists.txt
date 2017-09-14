cmake_minimum_required(VERSION 2.8)
project(myapp)

set(OpenCV_DIR "/home/lxg/app/opencv-3.3.0/build")
find_package(OpenCV REQUIRED)


add_definitions(-std=c++11) 
# find_package(OpenCV 3.3.0 REQUIRED)

add_executable(test1 main.cpp)
target_link_libraries(test1 ${OpenCV_LIBS})