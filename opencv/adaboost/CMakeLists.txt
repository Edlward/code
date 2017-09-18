cmake_minimum_required(VERSION 2.8)
project(myapp)

# set(OpenCV_DIR "/home/lxg/app/opencv-2.4.9/debug/")

add_definitions(-std=c++11) 
# find_package(OpenCV 3.3.0 REQUIRED)
find_package(OpenCV  REQUIRED)

add_executable(track boost.cpp)
target_link_libraries(track ${OpenCV_LIBS})

add_executable(detect detect.cpp)
target_link_libraries(detect ${OpenCV_LIBS} )