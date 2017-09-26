#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    MultiTracker trackers;
    vector<Rect> rois;
    vector<Rect2d> objects;

    VideoCapture cap;
    cap.open("/home/lxg/livet2.avi");
    if(!cap.isOpened())
    {
        printf("can not open video\n");
        return 0;
    }

    Mat im;
    char key;

    while(1)
    {
        cap >> im;
        if(im.empty())
        {
            printf("video end\n");
            break;
        }

        switch(key)
        {
        case 's':
            selectROIs("tracker", im, rois);
            trackers.add(im, rois);
            break;
            
        }
        
        if((key = waitKey(10)) == 27)
        {
            break;
        }

    }
}