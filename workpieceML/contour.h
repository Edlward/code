#ifndef _CONTOUR_H_
#define _CONTOUR_H_

#include <opencv2/opencv.hpp>
#include <vector>
#include "image.h"

using namespace std;
using namespace cv;

const float pi = 3.1415926;

struct Contourflg
{
    bool lengthflg;     //长度够长
    bool circleflg;     //是圆
    bool pureCirflg;    //是纯圆
    bool matchflg;      //匹配度
};

class ContourItem
{
public:
    // vector<Point> contour;
    float closeArea;
    float circleArea;
    float scaleArea;

    int sumObject;
    float ratioObject;

    Point2f center;
    float radius;
    float radiusOut;
    
    //模板匹配
    float matchRatio;

    Contourflg flg;

    void verifyCircle(const vector<Point> contour);
    void verifyPure(const Mat &binary);
};

class Contours
{
public:
    vector<vector<Point> > contours;
    vector<ContourItem> item;
    int maxNumber;
    const int minLength;
    CImage &image;  //声明成const，则其成员变量也需要是const

    bool insideFlg;
    //传的参数名称不能与成员变量名相同
    Contours(CImage &imageC):minLength(5),image(imageC)   {}
    void getContours();
    void computeProperty();
    void display(Mat colorIm, const string name[]);
};
#endif