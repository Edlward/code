cmake_minimum_required(VERSION 2.8)
project(detect)

message(STATUS ${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS -std=c++11)
# include_directories(${PROJECT_SOURCE_DIR}/inc/)
# include_directories(/home/lxg/codetest/hongruan/detect/inc)
# include_directories(inc/)


link_directories(${PROJECT_SOURCE_DIR}/lib/linux_x64)

add_executable(detect arcsoft_afd_samplecode.cpp)
target_link_libraries(detect arcsoft_fsdk_face_detection)