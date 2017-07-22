#include "driverPakage.h"


V4l2Camera::V4l2Camera()
{
    device_name = "/dev/video1";
    open_device(device_name.c_str(), &fd);
    
    io = IO_METHOD_MMAP;
    n_buffers = 4;
    init_device(&width, &height, fd, io, n_buffers, &buffer);
    start_capturing(fd, io, n_buffers, buffer);
}
V4l2Camera::~V4l2Camera()
{

}

int main(int argc, char **argv)
{
    V4l2Camera camera;
}