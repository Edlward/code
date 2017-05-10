#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <opencv2/opencv.hpp>
#include <string>


enum WINDOWSNAME
{
    SRCIMAGE = 0,
    GRAYIMAGE, 
    ADABINARY, 
    OTSUBINARY,
    BARBINARY,
    CONTOURIMAGE
};


void openCamera(cv::VideoCapture &cap);
void init(std::string windowsName[]);
double getThreshVal_Otsu_8u_mask(const cv::Mat src, const cv::Mat& mask);  

#endif