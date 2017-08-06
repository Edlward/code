#include <opencv2/opencv.hpp>
#include "camera.h"
#include "backstraction.h"

using namespace cv;

int main(int argc, char **argv)
{
    Camera camera;
    camera.open(argc, argv);

    namedWindow("src", 0);
    namedWindow("ffg", 0);

    Backstraction bs;
    Mat im;
    Mat result;
    char key;

    while(1)
    {
        camera.cap >> im;

        bs.subtraction(im, result);

        imshow("src", im);
        imshow("ffg", result);
        
        key = waitKey(1);
        if(key == 27)
        {
            break;
        }
    }
}