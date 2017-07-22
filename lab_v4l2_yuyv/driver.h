#ifndef _V4L2_DRIVER_H_
#define _V4L2_DRIVER_H_

#include <sys/types.h>

typedef enum {
    IO_METHOD_READ,
    IO_METHOD_MMAP,
    IO_METHOD_USERPTR,
} io_method;

struct BufferCell {
    void *start;
    size_t length;
};
typedef struct BufferCell Buffer;


void open_device(const char *name, int *fd);
void init_device(int *width, int *height, int fd, io_method io, unsigned int n_buffers, Buffer **buffers);
void start_capturing (int fd, io_method io, unsigned int n_buffers, Buffer * buffers );
int read_mmap_frame(int fd, unsigned int n_buffers, unsigned char *p_data, 
                    Buffer *buffers, struct v4l2_buffer *buf, int w, int h);

#endif