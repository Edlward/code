find_package(OpenCV REQUIRED)

add_subdirectory(libvibe)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

set(CMAKE_CXX_FLAGS -std=c++11)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})



add_executable(sample sample.cpp )
target_link_libraries(sample ${OpenCV_LIBS} vibe++)