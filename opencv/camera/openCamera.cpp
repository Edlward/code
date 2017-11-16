#include <string>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <string>

// 打开工业相机的错误提示
// VIDIOC_QUERYCTRL:input/output error：没有什么关系，opencv为了支持更多的摄像头，为更好的工作，部分信息是通过尝试的方式去获取，有些摄像头部分命令不支持，很正常。

using namespace cv;

int main(int argc, char **argv)
{
    std::string windowsName = "camerashow";
    namedWindow(windowsName, WINDOW_NORMAL);
    resizeWindow(windowsName, 800, 600);

    VideoCapture cap;
    int cnum = 0;
    if(argc > 1)
    {
        sscanf(argv[1], "%d", &cnum);
    }
    cap.open(cnum);
    // cap.open("rtsp://admin:admin@192.168.31.67:554/h264/ch1/sub/av_stream");
    // cap.open("rtsp://admin:admin@127.0.0.1:554/h264/ch1/sub/av_stream");
    // cap.open("rtsp://127.0.0.1:554/av_stream");
    // cap.open("http://127.0.0.1:8080/?action=stream");
    if(!cap.isOpened())
    {
        printf("can not open camera %d\n", cnum);
        return -1;
    }
    else
    {
        printf("camera open successfully\n");
    }

    // cap.set(CV_CAP_PROP_FRAME_WIDTH, 110 ); //不起作用
    // cap.set(CV_CAP_PROP_FRAME_HEIGHT, 100 );

    // opencv doesn't support this property set
    printf("cap_prop_mode: %f\n", cap.get(CAP_PROP_MODE));
    cap.set(CAP_PROP_MODE, CAP_MODE_GRAY);
    printf("cap_prop_mode: %f\n", cap.get(CAP_PROP_MODE));
    
    int mode = cap.get(CAP_PROP_MODE);
    printf("camera mode:%d\n", mode);

    
    int hue = cap.get(CV_CAP_PROP_HUE);
    printf("hue %d\n", hue);
    
    int exposure = cap.get(CV_CAP_PROP_AUTO_EXPOSURE);
    printf("exposure %d\n", exposure);
    // cap.set(CV_CAP_PROP_EXPOSURE, 0);

    int fps = cap.get(CV_CAP_PROP_FPS);
    printf("frames per second(fps): %d\n", fps);

    Mat src;
    cap >> src;

    printf("height:%d, width:%d，channels:%d\n", src.cols, src.rows, src.channels());
    printf("come into while\n");
    while(1)
    {
        // double time = (double)getTickCount(); 
        cap >> src;

        if(src.empty())
        {
            printf("can not get mat \n");
            break;
        }   
        imshow(windowsName, src);

        char key = waitKey(1);

        if(key == 27)
        {
            break;
        }
        else if(key == 's')
        {
            waitKey(0);
        }
    }
    
    cap.release();
    destroyAllWindows();
}

