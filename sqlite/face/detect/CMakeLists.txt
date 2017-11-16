cmake_minimum_required(VERSION 2.8)
project(detect)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

message(STATUS ${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS -std=c++11)
include_directories(${PROJECT_SOURCE_DIR}/inc)
link_directories(${PROJECT_SOURCE_DIR}/lib/linux_x64)
find_package(OpenCV)

add_executable(detect arcsoft_afd_samplecode.cpp)
target_link_libraries(detect arcsoft_fsdk_face_detection ${OpenCV_LIBS})

add_executable(detectCamera faceDetectVideo.cpp)
target_link_libraries(detectCamera arcsoft_fsdk_face_detection ${OpenCV_LIBS})