#ifndef _TEMP_H_
#define _TEMP_H_

#include <opencv2/opencv.hpp>

class Temp
{
public:
    cv::Mat tempGray;           //匹配模板
    float matchRatioThresh;     //匹配率阈值
    int displayNum;             //用于显示的匹配目标图像

    int radius;                 //匹配模板半径
    cv::Point2f center;         //匹配模板圆轮廓的中心坐标

    bool savePositive;          //保存正样本
    bool saveNegative;          //保存负样本，负样本形状各异，所以对于神经网络，制作负样本并没有用处，因为不可能识别负样本，
                                //但是对于回归算法是需要的  

    Temp():matchRatioThresh(0.15),savePositive(true),saveNegative(true) {}     
    void tempInit();            //模板读取初始化
    float matchTemp(const cv::Mat &im, cv::Point2f centerC, float radiusC, bool circleFlg);
};

#endif