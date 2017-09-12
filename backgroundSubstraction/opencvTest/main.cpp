#include "chain.h"
#include <opencv2/opencv.hpp>

using namespace cv;


int main()
{
    Fgvibe fgvibe;

    VideoCapture cap;
    cap.open(0);

    int w = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int h = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    IplImage *bg;
    // bg = cvCreateImage(cvSize(w,h), 8, 1);

    Mat msrc;
    while(1)
    {
        cap >> msrc;

        IplImage im = IplImage(msrc);
        cvShowImage("src", &im);
        fgvibe.Process(&im);
        bg = fgvibe.GetMask();
        cvShowImage("bg", bg);
        cvWaitKey(1);
    }
}