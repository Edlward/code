#include <vector>
#include "math.h"
#include "stdio.h"
#include "temp.h"
#include "contour.h"

using namespace cv;
using namespace std;

void Temp::tempInit()
{
    Mat temp = imread("temp.png");
    if(!temp.data)
    {
        printf("can not find template\n");
        exit(0);
    }

    // Mat tempGray;
    cvtColor(temp, tempGray, CV_BGR2GRAY);

    // 直方图归一化
    // equalizeHist(tempGray, tempGray);

    Mat binary;
    threshold(tempGray, binary, 0, 255, THRESH_OTSU);
    
    vector<vector<Point> > contours;
    findContours(binary, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    

    // if(contours.size() > 2)
    // {
    //     printf("template contour is wrong\n");
    //     exit(0);
    // }

    //选出最符合圆的轮廓
    float ratio = 1.f;
    int index = 0;

    for(int i = 0; i < contours.size(); ++i)
    {
        Point2f centerC;
        float radiusC;

        minEnclosingCircle(contours[i], centerC, radiusC); 
        
        float closeArea = contourArea(contours[i]);
        float circleArea = radiusC * radiusC * pi;

        if(fabs(closeArea / circleArea - 1.0f) < ratio)
        {
            ratio = fabs(closeArea / circleArea - 1.0f);
            this->center = centerC;
            this->radius = ceil(radiusC);
            index = i;
        }
    }

    //检验模板尺寸
    if(this->radius < 1)
    {
        printf("template is not suitable\n");
        printf("index:%d\n", index);
        printf("radius:%d center:x:%f y:%f\n", this->radius, center.x, center.y);
        exit(0);
    }
    printf("contours.size:%d\n", (int)contours.size());
    drawContours(temp, contours, index, Scalar(0,0,255));
    // imshow("contour", binary);
    imshow("contourImg", temp);    
    // waitKey(0);

    //为便于匹配时对齐，让center位于模板的中心
    // float radiusOut = radius * (1 + 0.5);
    // if(center.x - radiusOut < 0 ||center.y - radiusOut < 0
    //     ||center.x + radiusOut > tempGray.cols || center.y + radiusOut > tempGray.rows)
    // {
    //     printf("template roi is larger than the size of template\n");
    //     exit(0);
    // }
    // int x1 = center.x - radiusOut; 
    // int y1 = center.y - radiusOut;  
    // int x2 = center.x + radiusOut; 
    // int y2 = center.y + radiusOut;
    // Mat tmp(tempGray, Rect(x1, y1, x2 - x1, y2 - y1)); 
    // tmp.copyTo(tempGrayAlign);
}

float Temp::matchTemp(const Mat &im, Point2f centerC, float radiusC, bool circleFlg)
{
    float scaleC = 0.6;
    //检测到的轮廓固定的比原本的孔大，所以进行缩放
    radiusC = radiusC * 0.9;
    int radiusCtmp = ceil((radiusC * scaleC) > 2 ? radiusC * scaleC : 2);//外圆
    int radiusOut = radiusC + radiusCtmp; 
    int radiusOuter2 = radiusOut * radiusOut;

    //判断要匹配轮廓的位置是否过于偏 
    int radiusZone = radiusOut + 5;
    if((centerC.x - radiusZone) < 0 || (centerC.y - radiusZone) < 0
        ||(centerC.x + radiusZone) > im.cols || (centerC.y + radiusZone) > im.rows)
    {
        printf("matchTemp contour is too close to the edge of image\n");
        return -1;
    }

    //目标图像匹配区域尺寸选择
    Rect rect(centerC.x - radiusOut, centerC.y - radiusOut,
               radiusOut + radiusOut, radiusOut + radiusOut );
    Mat imRoi(im, rect);

    //将模板变得与比较图像相同比例， center也随之变化
    Mat tempGrayEualise;
    float scale = radiusC / this->radius;
    Point2f centerTmp = this->center * scale;
    // printf("radiusC:%f scale:%f\n", radiusC, scale);
    if(scale < 0.1)
    {
        printf("imRoi x:%d  y:%d  width:%d height:%d\t", rect.x, rect.y, rect.width, rect.height);
        printf("object Zone is too bigger than matchTmp\n");
        return -1;
    }
    resize(tempGray, tempGrayEualise, Size(), scale, scale);
    
    
    //模板匹配区域选择
    int leftUpX = centerTmp.x - radiusOut;
    int leftUpY = centerTmp.y - radiusOut;

    if(leftUpX < 0 || leftUpY < 0 
        || leftUpX + imRoi.cols > tempGrayEualise.cols
        || (leftUpY + imRoi.rows) > tempGrayEualise.rows)
    {
        printf("radiusC:%3.1f\t radiusOut:%d", radiusC, radiusOut);
        printf("imRoi x:%d  y:%d  width:%d height:%d \t", rect.x, rect.y, rect.width, rect.height);
        // printf("tmpRoi x:%d y:%d ", leftUpX, leftUpY, imRoi.cols, imRoi.rows);
        printf("match image is bigger than template\n");
        return -1;
    }
    Mat tmpRoi(tempGrayEualise, Rect( leftUpX, leftUpY, imRoi.cols, imRoi.rows));
  
    // imshow("tempRoi", tmpRoi);
    // imshow("imRoi", imRoi);
    // waitKey(0);
    // printf("imRoi x:%d  y:%d  \t width:%d height:%d\n", rect.x, rect.y, rect.width, rect.height);
    // printf("tmpRoi x:%d y:%d \t width:%d height:%d\n", leftUpX, leftUpY, imRoi.cols, imRoi.rows);



    //模板匹配度计算
    int sqdiff = 0;
    float normalI = 1;
    float normalT = 1;

    for(int i = 0; i < imRoi.rows; ++i)
    {
        const uchar *pI = imRoi.ptr<uchar>(i);
        const uchar *pT = tmpRoi.ptr<uchar>(i);

        for(int j = 0; j < imRoi.cols; ++j)
        {
            int length = (i - radiusOut) * (i - radiusOut) 
                        + (j - radiusOut) * (j - radiusOut);
            if(length < radiusOuter2)
            {
                sqdiff += (pI[j] - pT[j]) * (pI[j] - pT[j]);
                normalI += pI[j] * pI[j];
                normalT += pT[j] * pT[j];
            }
        }
    }
    printf("radiusC:%3.1f \t centerC x:%3.f,y%3.f \tdone\n", radiusC, centerC.x, centerC.y);
    float coefficient = sqdiff * 1.0f / sqrt(normalI * normalT);

    //显示匹配模板
    if(coefficient < matchRatioThresh)
    {
        char str[20];
        sprintf(str, "tmpRoi_%d", displayNum);
        // imshow(str,tmpRoi);
        sprintf(str, "imRoi_%d", displayNum);
        imshow(str, imRoi);
        displayNum ++;
    }
    
    //保存正样本
    static int saveNumP = 0;
    static int saveNumN = 0;
    
    char str[40];
    if(savePositive && (coefficient < matchRatioThresh) && circleFlg)
    {
        Mat sample;
        imRoi.copyTo(sample);
        resize(sample, sample, Size(40,40));
        sprintf(str, "positive//positive_%d.jpg", saveNumP);
        imwrite(str, sample);
        saveNumP++;        
    }
    else if((saveNegative && (coefficient > matchRatioThresh)) || !circleFlg)
    {
        Mat sample;
        imRoi.copyTo(sample);
        resize(sample, sample, Size(40,40));
        printf("sample.channel:%d\n", sample.channels());
        sprintf(str, "negative//negative_%d.bmp", saveNumN);
        imwrite(str, sample);
        saveNumN++;        
    }
    
    return coefficient;
}