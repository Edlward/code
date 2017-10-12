cmake_minimum_required(VERSION 2.8)
project(square)
find_package(OpenCV REQUIRED)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")


add_executable(find contour.cpp)
target_link_libraries(find ${OpenCV_LIBS})

add_executable(temp template.cpp)
target_link_libraries(temp ${OpenCV_LIBS})

add_executable(binary binary.cpp)
target_link_libraries(binary ${OpenCV_LIBS})

add_executable(project integral.cpp)
target_link_libraries(project ${OpenCV_LIBS})

add_executable(rgb rgb.cpp)
target_link_libraries(rgb ${OpenCV_LIBS})

add_executable(means means.cpp)
target_link_libraries(means ${OpenCV_LIBS})

add_executable(meandev meandev.cpp)
target_link_libraries(meandev ${OpenCV_LIBS})