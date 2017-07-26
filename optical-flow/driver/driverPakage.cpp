#include "driverPakage.h"
#include <stdio.h>

V4l2Camera::V4l2Camera()
{
    fd = -1;
    device_name = "/dev/video";
    
    io = IO_METHOD_MMAP;
    n_buffers = 5;
    buffer = NULL;
    
    buf.index = 0;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
}

bool V4l2Camera::open(int index)
{
    device_name = device_name + std::to_string(index);
    open_device(device_name.c_str(), &fd);
    init_device(&width, &height, fd, io, &n_buffers, &buffer);
    start_capturing(fd, io, n_buffers, buffer);
    return true;
}

V4l2Camera::~V4l2Camera()
{
    stop_capturing(fd, IO_METHOD_MMAP);
    uninit_device(IO_METHOD_MMAP, n_buffers, buffer); //free 会segment fault
    close_device(fd);
    // free(buffer);
}

void V4l2Camera::init(cv::Mat &src)
{
    im.create(height, width, CV_8UC1);
    src.create(height, width, CV_8UC1);
    
    printf("init height %d, width %d\n", im.rows, im.cols);
    // if(im.empty())
    // {
    //     printf("empty image after create\n");
    // }
}


bool V4l2Camera::get(cv::Mat &src)
{
    if(read_mmap_frame(fd, n_buffers, im.data, buffer, &buf, width, height))
    {
        return false;
    }
    
    swap(src, im);
    return true;
}

