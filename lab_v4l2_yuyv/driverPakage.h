#ifndef _DRIVERPACKAGE_H_
#define _DRIVERPACKAGE_H_

#include <string>
extern "C"
{
    #include "driver.h"
}

class V4l2Camera
{
public:
    V4l2Camera();
    ~V4l2Camera();

    int width;
    int height;
private:

    int fd;
    std::string device_name;
    io_method io;
    unsigned int n_buffers;
    Buffer *buffer;
};

#endif