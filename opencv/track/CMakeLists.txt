cmake_minimum_required(VERSION 2.8)
project(myapp)

set(OpenCV_DIR "/home/lxg/app/opencv-3.3.0/build/")

add_definitions(-std=c++11) 
find_package(OpenCV 3.3.0 REQUIRED)

# add_executable(multi Multi.cpp)
# target_link_libraries(multi ${OpenCV_LIBS})

add_executable(single singleTrack.cpp)
target_link_libraries(single ${OpenCV_LIBS})

add_executable(multi multiTrack.cpp)
target_link_libraries(multi ${OpenCV_LIBS})
