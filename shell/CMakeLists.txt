cmake_minimum_required(VERSION 2.8)
project(readline)
set(CMAKE_CXX_FLAGS -std=c++11)
add_executable(readlist readlist.cpp)

find_package(OpenCV REQUIRED)

add_executable(aflw aflwFace.cpp)
target_link_libraries(aflw ${OpenCV_LIBS})

add_executable(test1 gauss.cpp)
target_link_libraries(test1 ${OpenCV_LIBS})