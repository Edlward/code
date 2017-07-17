cmake_minimum_required(VERSION 2.8)
set(OpenCV_DIR "/home/lxg/app/opencv-2.4.9/build")

add_definitions(-std=c++11)
project( MotionDetect )
find_package( OpenCV REQUIRED)
set(SRC_LIST xtofAffine2D.cpp PilotData.c xtofCalcPyrlk.cpp xtofCameraCorrect.cpp xtofCameraHeight.cpp xtofFindCorner.cpp readSonar.c)


add_executable( of ${SRC_LIST} main.cpp opticalFlow.cpp tool.cpp blockMatch.cpp)
target_link_libraries( of ${OpenCV_LIBS})

