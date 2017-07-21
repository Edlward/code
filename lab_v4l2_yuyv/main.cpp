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
    cameraRead(&data, &w, &h);

    if(data == NULL)
    {
        printf("data is NULL in main\n");
        return 1;
    }

    Mat im(w, h, CV_8UC1, data);


    imshow("device", im);
    waitKey(0);
}