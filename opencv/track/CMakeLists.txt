cmake_minimum_required(VERSION 2.8)
project(myapp)

set(OpenCV_DIR "/home/lxg/app/opencv-3.3.0/build/")

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

add_definitions(-std=c++11) 
find_package(OpenCV 3.3.0 REQUIRED)

# add_executable(multi Multi.cpp)
# target_link_libraries(multi ${OpenCV_LIBS})

add_executable(single singleTrack.cpp)
target_link_libraries(single ${OpenCV_LIBS})

add_executable(multi multiTrack.cpp)
target_link_libraries(multi ${OpenCV_LIBS})

add_executable(circle circleTrack.cpp)
target_link_libraries(circle ${OpenCV_LIBS})

add_executable(camshift camShift.cpp)
target_link_libraries(camshift ${OpenCV_LIBS})

add_executable(meanShift meanShift.cpp)
target_link_libraries(meanShift ${OpenCV_LIBS})
