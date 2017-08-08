#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <opencv2/opencv.hpp>

class Camera{
public:
    cv::VideoCapture cap;
    Camera();
    ~Camera();
    void open(int argc, char **argv);
    void get(cv::Mat &im);
private:
    cv::Mat im_tmp;
};

#endif