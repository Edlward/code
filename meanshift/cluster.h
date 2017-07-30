#ifndef _CLUTER_H_
#define _CLUTER_H_

#include<vector>
#include <opencv2/opencv.hpp>

class Cluster
{
public:
    std::vector<cv::Point2f> data;
    void loadData(int num);
    void meanshift();

};

#endif