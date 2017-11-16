#ifndef _FACE_H_
#define _FACE_H_

#include <string>
#include "arcsoft_fsdk_face_detection.h"
#include "arcsoft_fsdk_face_recognition.h"
#include "merror.h"
#include <sqlite3.h>
#include <opencv2/opencv.hpp>

#include <vector>
#include <string>

struct FaceBox
{
    std::string name;
    float similar;
    cv::Rect box;
};

class Face
{
public:
    Face(std::string dbname, int w, int h);
    ~Face();

    bool detectAndRecognize(cv::Mat &im);
    void showFace(cv::Mat &im);

    std::vector<FaceBox> face_name;

private:
    int image_width;
    int image_height;

    // detector
    MByte *p_detect_workmem;
    MHandle h_detect_engine;
    ASVLOFFSCREEN input_img;
    int image_buf_len;

    // recognize
    MByte *p_recognize_workmem;
    MHandle h_recognize_engine;

    // sqlite
    sqlite3 *db;
    sqlite3_stmt *stmt; 
};

#endif