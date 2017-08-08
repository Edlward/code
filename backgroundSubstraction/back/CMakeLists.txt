find_package(OpenCV REQUIRED)
set(CMAKE_CXX_FLAGS -std=c++11)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

add_executable(bs main.cpp camera.cpp backstraction.cpp)
target_link_libraries(bs ${OpenCV_LIBS})