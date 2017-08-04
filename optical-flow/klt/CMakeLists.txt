cmake_minimum_required(VERSION 3.0)
project(opencvKLT)

find_package(OpenCV REQUIRED)

include_directories(..)
link_directories(..)

add_executable(opencvKLT main.cpp)
target_link_libraries(opencvKLT klt ${OpenCV_LIBS})

add_library(klt STATIC convolve.c error.c pnmio.c pyramid.c selectGoodFeatures.c 
            storeFeatures.c trackFeatures.c klt.c klt_util.c writeFeatures.c)