cmake_minimum_required(VERSION 2.8)
project(head)
set(OpenCV_DIR "/home/lxg/app/opencv-3.3.0/build")
find_package(OpenCV 3.2 REQUIRED)

message(STATUS 'found')
message(STATUS ${OpenCV_INCLUDE_DIRS})
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(a.out forward.cpp)
target_link_libraries(a.out ${OpenCV_LIBS})

add_executable(test1 main.cpp)
target_link_libraries(test1 ${OpenCV_LIBS})