#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <vector>
#include <getopt.h>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    MultiTracker trackers;
    vector<Rect> rois;

    VideoCapture cap;
    cap.open("/home/lxg/livet2.avi");
    if(!cap.isOpened())
    {
        printf("can not open video\n");
        return 0;
    }

    Mat im;
    char key;
    double time;

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
        case 'b':
        case 's':
            selectROIs("tracker", im, rois);
            
            vector<Ptr<Tracker> > algorithms;
            vector<Rect2d> objects;
        
            for(size_t i = 0; i < rois.size(); ++i)
            {
                algorithms.push_back(TrackerKCF::create());
                objects.push_back(rois[i]);
            }

            trackers.add(algorithms, im, objects);
            break;
        }

        time = (double)getTickCount();
        bool isfound = trackers.update(im);
        time = ((double)getTickCount() - time) / getTickFrequency();

        if(!isfound)
        {
            printf("object loss time: %f\n", time);
        }
        else
        {
            printf("track successfully time: %f\n", time);
        }

        for(size_t i = 0; i < trackers.getObjects().size(); ++i)
        {
            rectangle(im, trackers.getObjects()[i], Scalar(0,0,255));
        }
        
        imshow("tracker", im);
        if((key = waitKey(10)) == 'e')
        {
            break;
        }
    }
}