cmake_minimum_required(VERSION 2.8)
project(face)
set(CMAKE_CXX_FLAGS -std=c++11)

# face detect lib
include_directories(${PROJECT_SOURCE_DIR}/detect/inc/)
link_directories(${PROJECT_SOURCE_DIR}/detect/lib/linux_x64/)

# face recognize lib
include_directories(${PROJECT_SOURCE_DIR}/recognition/inc/)
link_directories(${PROJECT_SOURCE_DIR}/recognition/lib/linux_x64/)


find_package(OpenCV REQUIRED)
add_executable(face main.cpp  faceDetectHong.cpp faceRecognizeHong.cpp face.cpp)
target_link_libraries(face ${OpenCV_LIBS} arcsoft_fsdk_face_recognition arcsoft_fsdk_face_detection sqlite3)
