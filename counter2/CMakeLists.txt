cmake_minimum_required(VERSION 2.8)
project(customer)

set(OpenCV_DIR "/home/pi/opencv-3.3.0/build")
set(CMAKE_CXX_FLAGS -std=c++11)

find_package(OpenCV)
include_directories(cvBlob)

aux_source_directory(cvBlob SRC_LIST)

message(STATUS "test")

message(STATUS ${SRC_LIST})
message(STATUS ${SRC_LIST2})

add_executable(count ${SRC_LIST} ${SRC_LIST2})
target_link_libraries(count ${OpenCV_LIBS})
