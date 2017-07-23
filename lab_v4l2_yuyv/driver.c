/*
 *  V4L2 video capture example
 *
 *  This program can be used and distributed without restrictions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>          /* for videodev2.h */

#include <linux/videodev2.h>

#include "driver.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))

void open_device(const char *dev_name, int *fd)
{
    struct stat st;

    if (-1 == stat (dev_name, &st)) {
        fprintf (stderr, "Cannot identify '%s': %d, %s\n",
                 dev_name, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }

    if (!S_ISCHR (st.st_mode)) {
        fprintf (stderr, "%s is no device\n", dev_name);
        exit (EXIT_FAILURE);
    }

    *fd = open (dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

    if (-1 == *fd) {
        fprintf (stderr, "Cannot open '%s': %d, %s\n",
                 dev_name, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }
    printf("camera open successfully\n");
}

static int xioctl (int fd, int request, void *arg)
{
    int r;

    do r = ioctl (fd, request, arg);
    while (-1 == r && EINTR == errno);

    return r;
}

static void errno_exit (const char *s)
{
    fprintf (stderr, "%s error %d, %s\n",
             s, errno, strerror (errno));

    exit (EXIT_FAILURE);
}

static void init_mmap (int fd, unsigned int *n_buffers_tmp, Buffer **buffers )
{
    unsigned int n_buffers = *n_buffers_tmp;
    struct v4l2_requestbuffers req;

    CLEAR (req);

    req.count               = 4;
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory              = V4L2_MEMORY_MMAP;

    if (-1 == xioctl (fd, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            fprintf (stderr, "%s does not support "
                             "memory mapping\n", "dev_name");
            exit (EXIT_FAILURE);
        } else {
            errno_exit ("VIDIOC_REQBUFS");
        }
    }

    if (req.count < 2) {
        fprintf (stderr, "Insufficient buffer memory on %s\n",
                 "dev_name");
        exit (EXIT_FAILURE);
    }

    // 申请的只是用来存储指针的？？这就是映射
    *buffers = (Buffer *)calloc (req.count, sizeof (**buffers));
    printf("calloc %d sizeof(*buffers):%d\n", req.count, sizeof(**buffers));

    if (!buffers) {
        fprintf (stderr, "Out of memory\n");
        exit (EXIT_FAILURE);
    }

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        struct v4l2_buffer buf;

        CLEAR (buf);

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = n_buffers;

        if (-1 == xioctl (fd, VIDIOC_QUERYBUF, &buf))
            errno_exit ("VIDIOC_QUERYBUF");

        (*buffers)[n_buffers].length = buf.length;
        (*buffers)[n_buffers].start =
                mmap (NULL /* start anywhere */,
                      buf.length,
                      PROT_READ | PROT_WRITE /* required */,
                      MAP_SHARED /* recommended */,
                      fd, buf.m.offset);

        if (MAP_FAILED == (*buffers)[n_buffers].start)
            errno_exit ("mmap");
    }
    *n_buffers_tmp = req.count;
    printf("memory map successfullly\n");
}

void init_device(int *width, int *height,  int fd, io_method io, unsigned int *n_buffers, Buffer **buffers)
{
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    unsigned int min;

    if (-1 == xioctl (fd, VIDIOC_QUERYCAP, &cap)) {
        if (EINVAL == errno) {
            fprintf (stderr, "%s is no V4L2 device\n",
                     "dev_name");
            exit (EXIT_FAILURE);
        } else {
            errno_exit ("VIDIOC_QUERYCAP");
        }
    }
    else
	{
        printf("description of camera\n");
     	printf("\tdriver:\t\t%s\n",cap.driver);
     	printf("\tcard:\t\t%s\n",cap.card);
     	printf("\tbus_info:\t%s\n",cap.bus_info);
     	printf("\tversion:\t%d\n",cap.version);
     	printf("\tcapabilities:\t%x\n",cap.capabilities);
    }


    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        fprintf (stderr, "%s is no video capture device\n",
                 "dev_name");
        exit (EXIT_FAILURE);
    }
    else
    {
        printf("\tdevice support video capture\n");
    }

    if(!(cap.capabilities & V4L2_CAP_VIDEO_OUTPUT))
    {
        printf("\tdevice don't support video output\n");
    }
    else
    {
        printf("\tdevice support video output\n");
    }

    if(!(cap.capabilities & V4L2_CAP_VIDEO_OVERLAY))
    {
        printf("\tdevice don't support video overlay\n");
    }
    else
    {
        printf("\tdevice support video overlay\n");
    }
    

    //emu(enumerate) all support fmt
    struct v4l2_fmtdesc fmtdesc;

	fmtdesc.index=0;
	fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;  // type of the data stream
	// fmtdesc.type=V4L2_BUF_TYPE_VIDEO_OUTPUT;  //怎么会不支持这个
	// fmtdesc.type=V4L2_BUF_TYPE_VIDEO_OVERLAY;
	printf("Support format:\n");
	while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
	{
		printf("\t%d.%s  ",fmtdesc.index+1,fmtdesc.description);
		printf("\t%ud\n",fmtdesc.pixelformat);
		
		fmtdesc.index++;
	}



    switch (io) 
    {
        case IO_METHOD_READ:
            if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
                fprintf (stderr, "%s does not support read i/o\n",
                         "dev_name");
                exit (EXIT_FAILURE);
            }

            break;

        case IO_METHOD_MMAP:
        case IO_METHOD_USERPTR:
            if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
                fprintf (stderr, "%s does not support streaming i/o\n",
                         "dev_name");
                exit (EXIT_FAILURE);
            }

            break;
    }



    /* Select video input, video standard and tune here. */


    CLEAR (cropcap);

    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (0 == xioctl (fd, VIDIOC_CROPCAP, &cropcap)) {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* reset to default */

        if (-1 == xioctl (fd, VIDIOC_S_CROP, &crop)) {
            switch (errno) {
            case EINVAL:
                printf("Cropping not supported\n");
                break;
            default:
                /* Errors ignored. */
                break;
            }
        }
    } else {
        /* Errors ignored. */
        printf("error\n");
    }

    /*set video formate*/

    CLEAR (fmt);

    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = 640;
    fmt.fmt.pix.height      = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

    if (-1 == xioctl (fd, VIDIOC_S_FMT, &fmt))
        errno_exit ("VIDIOC_S_FMT");

    /* Note VIDIOC_S_FMT may change width and height. */

    /* Buggy driver paranoia. */
    min = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < min)
        fmt.fmt.pix.bytesperline = min;
    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if (fmt.fmt.pix.sizeimage < min)
        fmt.fmt.pix.sizeimage = min;

	// g->get
	if(ioctl(fd, VIDIOC_G_FMT, &fmt) == -1)
	{
		printf("Unable to get format\n");
		return ;
	} 
	else
	{
		printf("camera property after set\n");
     	printf("\tfmt.type:\t\t%d\n",fmt.type);
     	printf("\tpix.pixelformat:\t%c%c%c%c\n",fmt.fmt.pix.pixelformat & 0xFF, (fmt.fmt.pix.pixelformat >> 8) & 0xFF,(fmt.fmt.pix.pixelformat >> 16) & 0xFF, (fmt.fmt.pix.pixelformat >> 24) & 0xFF);
     	printf("\tpix.height:\t\t%d\n",fmt.fmt.pix.height);
     	printf("\tpix.width:\t\t%d\n",fmt.fmt.pix.width);
     	printf("\tpix.field:\t\t%d\n",fmt.fmt.pix.field);
	}

    switch (io) {
    case IO_METHOD_READ:
        // init_read (fmt.fmt.pix.sizeimage, buffers);
        break;

    case IO_METHOD_MMAP:
        init_mmap (fd, n_buffers, buffers);
        break;

    case IO_METHOD_USERPTR:
        // init_userp (fmt.fmt.pix.sizeimage, fd, n_buffers, buffers);
        break;
    }
    *width = fmt.fmt.pix.width;
    *height = fmt.fmt.pix.height;
    printf("device init successfully\n");
}

void start_capturing (int fd, io_method io, unsigned int n_buffers, Buffer *buffers )
{
    unsigned int i;
    enum v4l2_buf_type type;

    switch (io) {
    case IO_METHOD_MMAP:
        for (i = 0; i < n_buffers; ++i) {
            struct v4l2_buffer buf;

            CLEAR (buf);

            buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory      = V4L2_MEMORY_MMAP;
            buf.index       = i;

            if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
                errno_exit ("VIDIOC_QBUF");
        }

        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (-1 == xioctl (fd, VIDIOC_STREAMON, &type))
            errno_exit ("VIDIOC_STREAMON");

        break;

    case IO_METHOD_USERPTR:
        for (i = 0; i < n_buffers; ++i) {
            struct v4l2_buffer buf;

            CLEAR (buf);

            buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory      = V4L2_MEMORY_USERPTR;
            buf.m.userptr   = (unsigned long) buffers[i].start;
            buf.length      = buffers[i].length;

            if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
                errno_exit ("VIDIOC_QBUF");
        }


        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (-1 == xioctl (fd, VIDIOC_STREAMON, &type))
            errno_exit ("VIDIOC_STREAMON");

        break;
    case IO_METHOD_READ:
        /* Nothing to do. */
        break;
    }
    printf("start capture\n");
}
static void process_image (const void *p)
{
    fputc ('.', stdout);
    fflush (stdout);
}

static int read_frame (int fd, io_method io, unsigned int n_buffers, Buffer *buffers )
{
    struct v4l2_buffer buf;
    unsigned int i;

    switch (io) 
    {
        case IO_METHOD_MMAP:
            CLEAR (buf);

            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;

            if (-1 == xioctl (fd, VIDIOC_DQBUF, &buf)) {
                switch (errno) {
                case EAGAIN:
                    return 0;

                case EIO:
                    /* Could ignore EIO, see spec. */

                    /* fall through */

                default:
                    errno_exit ("VIDIOC_DQBUF");
                }
            }

            assert (buf.index < n_buffers);

            process_image (buffers[buf.index].start);

            if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
                errno_exit ("VIDIOC_QBUF");

            break;

        case IO_METHOD_USERPTR:
            CLEAR (buf);

            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_USERPTR;

            if (-1 == xioctl (fd, VIDIOC_DQBUF, &buf)) {
                switch (errno) {
                case EAGAIN:
                    return 0;

                case EIO:
                    /* Could ignore EIO, see spec. */

                    /* fall through */

                default:
                    errno_exit ("VIDIOC_DQBUF");
                }
            }

            for (i = 0; i < n_buffers; ++i)
                if (buf.m.userptr == (unsigned long) buffers[i].start
                        && buf.length == buffers[i].length)
                    break;

            assert (i < n_buffers);

            process_image ((void *) buf.m.userptr);

            if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
                errno_exit ("VIDIOC_QBUF");

            break;

        case IO_METHOD_READ:
            if (-1 == read (fd, buffers[0].start, buffers[0].length)) {
                switch (errno) {
                case EAGAIN:
                    return 0;

                case EIO:
                    /* Could ignore EIO, see spec. */

                    /* fall through */

                default:
                    errno_exit ("read");
                }
            }

            process_image (buffers[0].start);

            break;
    }

    return 1;
}

int read_mmap_frame(int fd, unsigned int n_buffers, unsigned char *p_data, 
                    Buffer *buffers, struct v4l2_buffer *buf,
                    int w, int h)
{
    printf("read mmap successfully\n");
    
    if (-1 == xioctl (fd, VIDIOC_DQBUF, buf)) 
    {
        switch (errno) {
        case EAGAIN:
            return -1;
        case EIO:
            /* Could ignore EIO, see spec. */
            /* fall through */
        default:
            errno_exit ("VIDIOC_DQBUF");
        }
    }
    assert (buf->index < n_buffers);
    
    //yuyv2gray
	unsigned char *p2 = (unsigned char *)buffers[buf->index].start;
	for(int i = 0; i < h; ++i)
	{
		for(int j = 0; j < w; ++j)
		{
			*p_data = *p2;
			++p_data;
			++p2;
			++p2;
		}
	}


    if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
        errno_exit ("VIDIOC_QBUF");    
    printf("read mmap successfully\n");
    return 0;
}


void stop_capturing(int fd, io_method io)
{
    enum v4l2_buf_type type;

    switch (io) {
    case IO_METHOD_READ:
        /* Nothing to do. */
        break;

    case IO_METHOD_MMAP:
    case IO_METHOD_USERPTR:
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (-1 == xioctl (fd, VIDIOC_STREAMOFF, &type))
            errno_exit ("VIDIOC_STREAMOFF");

        break;
    }
}


void uninit_device(io_method io, unsigned int n_buffers, Buffer *buffers )
{
    unsigned int i;

    switch (io) {
    case IO_METHOD_READ:
        free (buffers[0].start);
        break;

    case IO_METHOD_MMAP:
        for (i = 0; i < n_buffers; ++i)
            if (-1 == munmap (buffers[i].start, buffers[i].length))
                errno_exit ("munmap");
        // free(buffers);
        break;

    case IO_METHOD_USERPTR:
        for (i = 0; i < n_buffers; ++i)
            free (buffers[i].start);
        break;
    }

    free (buffers);
}

void close_device(int fd)
{
    printf("close fd:%d\n", fd);

    if (-1 == close (fd))
        errno_exit ("close");
}