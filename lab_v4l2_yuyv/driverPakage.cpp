#include "driverPakage.h"
#include <stdio.h>

V4l2Camera::V4l2Camera()
{
    fd = -1;
    device_name = "/dev/video1";
    open_device(device_name.c_str(), &fd);
    
    io = IO_METHOD_MMAP;
    n_buffers = 4;
    buffer = NULL;
    
    buf.index = 0;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    init_device(&width, &height, fd, io, &n_buffers, &buffer);
    start_capturing(fd, io, n_buffers, buffer);
}

V4l2Camera::~V4l2Camera()
{
    stop_capturing(fd, IO_METHOD_MMAP);
    uninit_device(IO_METHOD_MMAP, n_buffers, buffer); //free 会segment fault
    close_device(fd);
    // free(buffer);
}

void V4l2Camera::init()
{
    //  cv::imread("/home/lxg/codedata/test.jpg");
    im.create(height, width, CV_8UC1);
    // printf("init height %d, width %d\n", im.rows, im.cols);
    // if(im.empty())
    // {
    //     printf("empty image after create\n");
    // }
}

bool V4l2Camera::grab()
{
    if(im.data == NULL)
    {
        printf("im.data is null\n");
    }
    return read_mmap_frame(fd, n_buffers, im.data, buffer, &buf, width, height);
}

void V4l2Camera::get(cv::Mat &src)
{
    printf("src height %d, width %d\n", im.rows, im.cols);
    swap(src, im);
    // im.copyTo(src);
}

int main(int argc, char **argv)
{
    V4l2Camera camera;
    camera.init();
    camera.grab();

    cv::Mat src;
    src.create(camera.height, camera.width, CV_8UC1);
    
    int count = 0;
    while(1)
    {
        if(camera.grab())
        {
            // printf("error\n");
            // continue;
        }
        // camera.get(src);

        cv::imshow("camera", camera.im);
        char key = cv::waitKey(1);
    
        ++count;
        printf("%d\n", count);
        if(key == 27)
        {
            break;
        } 
    }
}