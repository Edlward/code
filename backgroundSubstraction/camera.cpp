#include "camera.h"

using namespace cv;

Camera::Camera()
{

}

Camera::~Camera()
{
    if(cap.isOpened())
    {
        cap.release();
    }
}

void Camera::open(int argc, char **argv)
{
    int num = 0;
    if(argc == 2)
    {
        num = 1;
    }

    cap.open(num);
    if(!cap.isOpened())
    {
        throw std::runtime_error("can not open camera");
    }
}