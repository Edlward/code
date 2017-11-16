#ifndef _FACE_H_
#define _FACE_H_

#include <string>
#include <opencv2/opencv.hpp>
#include "faceDetect.h"
#include "faceRecognize.h"

class Face
{
public:    
    Face(std::string dbname, int w, int h);
    ~Face();
    
    bool detectAndRecognize(cv::Mat &im);

private:
    FaceDetect *detector;
    FaceRecognize *recognizer;
};

#endif