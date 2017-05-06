#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>

using namespace std;
using namespace cv;

enum MYCOLORSPACE
{
    SRC = 0,
    YUV,
    YCRCB
};


int main(int argc, char **argv)
{
    string name[5];
    name[SRC] = "src";
    name[YUV] = "yuv";
    name[YCRCB] = "ycrcb";

    namedWindow(name[SRC], WINDOW_NORMAL);
    resizeWindow(name[SRC], 600, 500);
    moveWindow(name[SRC], 0, 0);

    namedWindow(name[YUV], WINDOW_NORMAL);
    resizeWindow(name[YUV], 600, 500);
    moveWindow(name[YUV], 400, 0);

    namedWindow(name[YCRCB], WINDOW_NORMAL);
    resizeWindow(name[YCRCB], 600, 500);
    moveWindow(name[YCRCB], 400, 0);


    Mat src;
    src = imread("test.jpg");
    resize(src, src, Size(600,500));

    if(!src.data)
    {
        printf("can not open image\n");
        return -1;
    }

    // imshow(name[SRC], src);


    Mat yuv;
    vector<Mat> yuvS;
    cvtColor(src, yuv, COLOR_BGR2YUV);
    split(yuv, yuvS);

    imshow(name[YUV] + "_y", yuvS[0]);
    imshow(name[YUV] + "_u", yuvS[1]);
    imshow(name[YUV] + "_v", yuvS[2]);

    Mat ycrcb;
    vector<Mat> ycrcbS;
    cvtColor(src, ycrcb, COLOR_BGR2YCrCb);
    split(ycrcb, ycrcbS);

    imshow(name[YCRCB] + "_y", ycrcbS[0]);
    imshow(name[YCRCB] + "_u", ycrcbS[1]);
    imshow(name[YCRCB] + "_v", ycrcbS[2]);


    waitKey(0);
}