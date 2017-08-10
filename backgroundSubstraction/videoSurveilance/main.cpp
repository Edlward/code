#include <opencv2/opencv.hpp>
// #include "countBlobTrack.h"
#include <opencv2/legacy/blobtrack.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace cv;

int main(int argc, char **argv)
{
    VideoCapture cap;
    cap.open(0);
    if(!cap.isOpened())
    {
        printf("can not open camera\n");
    }

    // CvBlobTrackerAutoParam1 param;
    // param.FGTrainFrames = 2;
    // CvBlobTrackerAuto *track =  cvCreateBlobTrackerAuto1(&param);
    
    CvFGDetector *fg = cvCreateFGDetectorBase(CV_BG_MODEL_FGD, NULL);
    // CvFGDetector *fg = cvCreateFGDetectorBase(CV_BG_MODEL_MOG, NULL);
    // CvFGDetector *fg = cvCreateFGDetectorBase(CV_BG_MODEL_FGD_SIMPLE, NULL);
    

    Mat im;
    char key;

    while(1)
    {
        double t = (double)getTickCount();

        cap >> im;
        // cvtColor(im, im, CV_RGB2GRAY);

        IplImage pimg;
        pimg = IplImage(im);

        // track->Process(&ipl);
        fg->Process(&pimg);

        IplImage *ptmp = NULL;
        ptmp = fg->GetMask();

        if(ptmp == NULL)
        {
            printf("empty mask\n");
        }
        cvNamedWindow("mask");
        cvShowImage("mask", ptmp);
        waitKey(1);
        
        t = ((double)getTickCount() - t) / getTickFrequency();
        // printf("%d\n", pimg.width);
        printf("time: %f\n", t);

        imshow("src", im);
        key = waitKey(1);
        if(key == 27)
        {
            break;
        }
    }
}


