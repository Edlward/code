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

    std::string video = "/home/lxg/codedata/customer/test.mp4";
    cap.open(video.c_str());
    if(!cap.isOpened())
    {
        throw std::runtime_error("can not open camera");
    }
}

void Camera::get(Mat &im)
{
    cap >> im_tmp;
    
    cvtColor(im_tmp, im, CV_RGB2GRAY);
}