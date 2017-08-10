find_package(OpenCV REQUIRED)


set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

set(CMAKE_CXX_FLAGS -std=c++11)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

add_executable(vs main.cpp )
target_link_libraries(vs ${OpenCV_LIBS})