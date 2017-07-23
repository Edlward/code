#ifndef _DRIVERPACKAGE_H_
#define _DRIVERPACKAGE_H_

#include <string>
#include <opencv2/opencv.hpp>
#include <linux/videodev2.h>

extern "C"
{
    #include "driver.h"
};

class V4l2Camera
{
public:
    int width;
    int height;
    cv::Mat im;

    
    void init();
    bool grab();
    void get(cv::Mat &src);
    V4l2Camera();
    ~V4l2Camera();
private:

    int fd;
    std::string device_name;
    io_method io;
    unsigned int n_buffers;
    Buffer *buffer;
    struct v4l2_buffer buf;
    
};

#endif