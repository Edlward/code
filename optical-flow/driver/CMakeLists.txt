cmake_minimum_required(VERSION 2.8)
project(myapp)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

# set(OpenCV_DIR "/home/lxg/app/opencv-2.4.9/build")
# 针对每一个目录都需要单独设置这个变量
set(CMAKE_C_FLAGS -std=c99)
set(CMAKE_CXX_FLAGS -std=c++11)


find_package(OpenCV REQUIRED)

add_executable(openCamera main.cpp v4l2grab.c)
target_link_libraries(openCamera ${OpenCV_LIBS})

add_executable(driver driverMain.cpp driver.c driverPakage.cpp)
target_link_libraries(driver ${OpenCV_LIBS})

add_library(cameraDriver STATIC driver.c driverPakage.cpp)
target_link_libraries(cameraDriver ${OpenCV_LIBS})



