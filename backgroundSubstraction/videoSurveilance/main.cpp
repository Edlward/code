#include <opencv2/opencv.hpp>
#include "countBlobTrack.h"
// #include <opencv2/legacy/blobtrack.hpp>

using namespace cv;

int main(int argc, char **argv)
{
    VideoCapture cap;
    cap.open(0);
    if(!cap.isOpened())
    {
        printf("can not open camera\n");
    }

    CvBlobTrackerAutoParam1 param;
    param.FGTrainFrames = 2;
    CvBlobTrackerAuto *track =  cvCreateBlobTrackerAuto2(&param);
    
    Mat im;
    char key;

    while(1)
    {
        cap >> im;
        cvtColor(im, im, CV_RGB2GRAY);
        IplImage ipl(im);

        track->Process(&ipl);
        
        key = waitKey(1);
        if(key == 27)
        {
            break;
        }
    }
}


