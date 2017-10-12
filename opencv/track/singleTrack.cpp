#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <cstring>
#include <iostream>
#include <getopt.h>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    char ch;
    char *video = NULL;

    while((ch = getopt(argc, argv, "v:")) != -1)
    {
        switch(ch)
        {
        case 'v':
            video = optarg;
            printf("video file %s\n", optarg);
            break;
        }
    }
    if(video == NULL)
    {
        // video = (char *)"/home/lxg/livet1.avi";
        video = (char *)"/home/lxg/codedata/square/s1.avi";
    }

    VideoCapture cap;
    cap.open(video);
    if(!cap.isOpened())
    {
        printf("can not open video %s\n", video);
        return 0;
    }

    Ptr<Tracker> tracker;
    tracker = TrackerKCF::create();
    
    Mat im;
    char key = 0;
    Rect2d roi;
    double time;

    while(1)
    {
        cap >> im;
        if(im.empty())
        {
            printf("video ending \n");
            break;
        }

        switch(key)
        {
        case 'b':
        case 's':
            printf("please select an aera to track \n");
            roi = selectROI("tracker", im, true, false);
            if(roi.width == 0 || roi.height == 0)
            {
                printf("aera select wrong\n");
                break;
            }
            tracker.~Ptr();
            tracker = TrackerKCF::create();
            tracker->init(im, roi);
        }

        time = (double)getTickCount();
        bool isfound = tracker->update(im, roi);
        time = ((double)getTickCount() - time) / getTickFrequency();
        if(!isfound)
        {
            printf("object loss time: %f\n", time);
        }
        else
        {
            printf("track successfully time: %f\n", time);
        }

        rectangle(im, roi, Scalar(255, 0, 0), 2, 1);
        imshow("tracker", im);
        if((key = waitKey(30)) == 27) 
        {
            break;
        }
    }
}