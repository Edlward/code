#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// 读取鼠标的相对位置
int main(int argc, char **argv)
{
    int fd,retval;
    char buf[6];
    struct timeval tv;
    fd_set readfds;

    fd = open("/dev/input/mice", O_RDONLY);
    if(fd < 0)
    {
        printf("failed to open mice\n");
        return -1;
    }    
    else
    {
        printf("open mice successfully\n");
    }

    while(1)
    {
        // 设置最长等待时间
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        retval = select(fd+1, &readfds, NULL, NULL, &tv);
        if(retval == 0)
        {
            printf("time out\n");
        }
        if(FD_ISSET(fd, &readfds))
        {
            if(read(fd, buf, 6) <= 0)
            {
                continue;
            }
            printf("button type=%d, x=%d, y=%d,z=%d\n", (buf[0]&0x07), buf[1], buf[2], buf[3]);
        }
    }
    close(fd);
}