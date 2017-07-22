#include "driverPakage.h"
#include <stdio.h>

V4l2Camera::V4l2Camera()
{
    device_name = "/dev/video1";
    open_device(device_name.c_str(), &fd);
    
    io = IO_METHOD_MMAP;
    n_buffers = 4;

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
}

V4l2Camera::~V4l2Camera()
{

}

void V4l2Camera::init()
{
    init_device(&width, &height, fd, io, n_buffers, &buffer);
    start_capturing(fd, io, n_buffers, buffer);
    im.create(width, height, CV_8UC1);
}

bool V4l2Camera::grab()
{
    return read_mmap_frame(fd, n_buffers, im.data, buffer, &buf, width, height);
}

void V4l2Camera::get(cv::Mat &src)
{
    printf("src height %d, width %d\n", src.rows, src.cols);
    swap(src, im);
}

int main(int argc, char **argv)
{
    V4l2Camera camera;
    camera.init();

    cv::Mat src;
    int count = 0;
    while(1)
    {
        if(camera.grab())
        {
            // printf("error\n");
            continue;
        }
        camera.get(src);

        // cv::imshow("camera", src);
        cv::waitKey(0);

        ++count;
        if(count > 3)
        {
            break;
        } 
    }
}