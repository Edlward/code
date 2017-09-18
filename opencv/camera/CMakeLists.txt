cmake_minimum_required(VERSION 2.8)
project(myapp)

# set(OpenCV_DIR "/home/lxg/app/opencv-2.4.9/debug/")

add_definitions(-std=c++11) 
find_package(OpenCV 2.4.9 REQUIRED)

add_executable(openCamera openCamera.cpp)
target_link_libraries(openCamera ${OpenCV_LIBS})

add_executable(saveCamera saveCamera.cpp)
target_link_libraries(saveCamera ${OpenCV_LIBS})

