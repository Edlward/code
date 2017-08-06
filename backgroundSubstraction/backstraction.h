#ifndef _BACKSTRACTION_H_
#define _BACKSTRACTION_H_

#include <opencv2/opencv.hpp>

class Backstraction
{
public:
    cv::Mat f2; // frame old
    Backstraction();
    
    void prepareBackFrame(const cv::Mat &src);
    void subtraction(const cv::Mat &f1, cv::Mat &result);
};
#endif