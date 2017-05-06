cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( myapp main.cpp )
target_link_libraries( myapp ${OpenCV_LIBS} )

add_executable( match template.cpp )
target_link_libraries( match ${OpenCV_LIBS} )