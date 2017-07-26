#include "driverPakage.h"
#include <stdio.h>

#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{
    cv::Mat src;

    V4l2Camera camera;
    camera.open(1);
    camera.init(src);
    camera.get(src);

    printf("src height %d, width %d\n", src.rows, src.cols);
    
    int count = 0;
    double t = 0.f;

    while(1)
    {
        if(camera.get(src))
        {
            printf("error\n");
            continue;
        }

        cv::imshow("camera", src);

        t = 1 / (((double)cv::getTickCount() - t) / cv::getTickFrequency());
        printf("fps:%3.2f\t", t);
        printf("%d\n", count);
        char key = cv::waitKey(1);
        ++count;
        if(key == 27 || count > 3000)
        {
            break;
        } 
        t = (double)cv::getTickCount();
        
    }
}