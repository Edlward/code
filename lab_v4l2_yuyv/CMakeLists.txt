cmake_minimum_required(VERSION 2.8)
project(myapp)
set(OpenCV_DIR "/home/lxg/app/opencv-2.4.9/build")

#add_definitions(-std=c++11) 
find_package(OpenCV REQUIRED)

add_executable(openCamera main.cpp v4l2grab.c)
target_link_libraries(openCamera ${OpenCV_LIBS})

add_executable(driver driver.c driverPakage.cpp)


