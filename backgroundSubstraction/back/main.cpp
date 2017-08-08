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

    resizeWindow("src", 640, 400);
    resizeWindow("ffg", 640, 400);
    

    Backstraction bs;
    Mat im;
    Mat result;
    char key;

    while(1)
    {
        camera.get(im);

        bs.subtraction(im, result);

        imshow("src", im);
        imshow("ffg", result);
        
        key = waitKey(2);
        if(key == 27)
        {
            break;
        }
    }
}