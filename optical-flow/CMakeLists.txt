cmake_minimum_required(VERSION 2.8)
project( MotionDetect )
find_package( OpenCV REQUIRED)
set(SRC_LIST xtofAffine2D.cpp PilotData.c xtofCalcPyrlk.cpp xtofCameraCorrect.cpp xtofCameraHeight.cpp xtofFindCorner.cpp readSonar.c)

add_executable( MotionDetect ${SRC_LIST} MotionDetect.cpp )
target_link_libraries( MotionDetect ${OpenCV_LIBS})

add_executable( of ${SRC_LIST} main.cpp)
target_link_libraries( of ${OpenCV_LIBS})

