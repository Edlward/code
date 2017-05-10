#include "image.h"
#include "stdio.h"
#include "tools.h"

void CImage::imageConvert()
{
    float scale = 1.0f * width /src.cols;
    printf("\nnew image src.cols:%d, src.rows:%d\n\n", (int)src.cols, (int)src.rows);
    // resize(src, src, Size(), scale, scale);

    cvtColor(src, gray, CV_BGR2GRAY);

    //滤波
    GaussianBlur(gray, gray, Size(5, 5), 1.5);

    //otsu阈值
    threshold(gray, otsuBinary, 0, 255, THRESH_OTSU);
    // float thresh;
    // thresh = getThreshVal_Otsu_8u_mask(gray, Mat());  
    // threshold(gray, otsuBinary, thresh - 10, 255, THRESH_BINARY);

    //对otsu做腐蚀
    Mat elem;
    elem = getStructuringElement(MORPH_RECT, Size(5,5), Point(-1, -1));
    erode(otsuBinary, otsuBinary, elem, Point(-1,-1));
    
    //自适应阈值
    //blockSize必须是奇数
    adaptiveThreshold(gray, adaBinary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 13, 10);
    
    // printf("imageConver test\n");
}
