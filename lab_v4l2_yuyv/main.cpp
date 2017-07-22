#include <opencv2/opencv.hpp>
using namespace cv;

extern "C"
{
    #include "v4l2grab.h"
}
int main()
{
    namedWindow("device", WINDOW_AUTOSIZE);

    unsigned char *data =NULL;
    int w = 0, h = 0;

    if(!init_v4l2())
    {
        printf("v4l2 init fail\n");
        return 1;
    }

    char key;
    int count = 0;
    while(1)
    {
        double t = (double)getTickCount();
        cameraRead(&data, &w, &h);
        t = 1.f / (((double)getTickCount()-t) / getTickFrequency());
        printf("fps:%3.f\n", t);
        if(data == NULL)
        {
            printf("data is NULL in main\n");
            return 1;
        }

        Mat im(h, w, CV_8UC1, data);


        imshow("device", im);
        key = waitKey(1);
        if(key == 27 || count > 2)
        {
            break;
        }
        ++count;
    }
    close_v4l2();
}