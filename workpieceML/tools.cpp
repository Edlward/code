#include "tools.h"
#include "stdio.h"
#include "image.h"

using namespace cv;

void openCamera(VideoCapture &cap)
{
    cap.open(0);
    if(!cap.isOpened())
    {
        printf("can not open camera\n");
        exit(0);
    }
    else
    {
        printf("open camera successfully\n");
    }
}
void init(std::string windowsName[])
{
    windowsName[SRCIMAGE] = "srcImage";
    windowsName[GRAYIMAGE] = "grayImage";
    windowsName[ADABINARY] = "adaImage";
    windowsName[OTSUBINARY] = "otsuImage";
    windowsName[BARBINARY] = "barImage";
    windowsName[CONTOURIMAGE] = "contour";

    int winWidth = 600;
    int winHeight = 500;
    int biasW = 40;

    namedWindow(windowsName[SRCIMAGE], WINDOW_NORMAL);
    resizeWindow(windowsName[SRCIMAGE], winWidth, winHeight);
    moveWindow(windowsName[SRCIMAGE], 0, 0);

    namedWindow(windowsName[GRAYIMAGE], WINDOW_NORMAL);
    resizeWindow(windowsName[GRAYIMAGE], winWidth, winHeight);
    moveWindow(windowsName[GRAYIMAGE], winWidth + biasW, 0);

    namedWindow(windowsName[OTSUBINARY], WINDOW_NORMAL);
    resizeWindow(windowsName[OTSUBINARY], winWidth, winHeight);
    moveWindow(windowsName[OTSUBINARY], winWidth * 2 + biasW, 0);

    namedWindow(windowsName[ADABINARY], WINDOW_NORMAL);
    resizeWindow(windowsName[ADABINARY], winWidth, winHeight);
    moveWindow(windowsName[ADABINARY], 0, 0);
    

    namedWindow(windowsName[CONTOURIMAGE], WINDOW_NORMAL);
    resizeWindow(windowsName[CONTOURIMAGE], winWidth, winHeight);
    moveWindow(windowsName[CONTOURIMAGE], 0, 0);
    // moveWindow(windowsName[CONTOURIMAGE], winWidth *2 + biasW, winHeight + biasW);

}


double getThreshVal_Otsu_8u_mask(const Mat src, const Mat& mask)  
{  
    const int N = 256;  
    int M = 0;  
    int i, j, h[N]={0};  
    for( i=0; i<src.rows ; i++ )  
    {  
        const uchar* psrc = src.ptr(i);  
        const uchar* pmask = mask.ptr(i);  
        for( j=0; j<src.cols ; j++ )  
        {  
           // if( pmask[j] )  
            {  
                h[ psrc[j] ]++;  
                ++M;  
            }  
        }  
    }  
  
    double mu =0, scale = 1. /(M);  
    for(i=0; i<N; i++)  
        mu += i * (double)h[i];  
      
    mu *= scale;  
    double mu1 = 0, q1 = 0;  
    double max_sigma = 0, max_val = 0;  
  
    for(i=0; i<N; i++)  
    {  
        double p_i, q2, mu2, sigma;  
        p_i = h[i] * scale;  
        mu1 *= q1;  
        q1 += p_i;  
        q2 = 1. -q1;  
  
        if(std::min(q1, q2) < FLT_EPSILON || std::max(q1, q2) > 1. - FLT_EPSILON)  
            continue;  
  
        mu1 = (mu1 + i*p_i) / q1;  
        mu2 = (mu - q1*mu1) / q2;  
        sigma = q1 * q2 * (mu1-mu2)*(mu1-mu2);  
        if( sigma > max_sigma )  
        {  
            max_sigma = sigma;  
            max_val = i;  
        }  
    }  
  
    return max_val;  
}