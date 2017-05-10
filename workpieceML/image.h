#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "temp.h"
#include <opencv2/opencv.hpp>

using namespace cv;



class CImage
{
public:
    Mat src;            //原图
    Mat gray;           //灰度图
    Mat adaBinary;      //自适应阈值图
    Mat otsuBinary;     //otsu阈值图
    Mat barBinary;      //滑动条目阈值图 

    const int width;
    Temp temp;
    
    CImage(): width(500){}
    void imageConvert();
    
};

#endif