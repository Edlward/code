#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
extern "C"
{
    #include "klt.h"
}
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
    if(!cap.isOpened())
    {
        printf("can not open camera %d\n", cnum);
        return -1;
    }
    // opencv doesn't support this property set
    // printf("cap_prop_mode: %f\n", cap.get(CAP_PROP_MODE));
    // cap.set(CAP_PROP_MODE, CAP_MODE_GRAY);
    // printf("cap_prop_mode: %f\n", cap.get(CAP_PROP_MODE));
    // int mode = cap.get(CAP_PROP_MODE);
    // printf("camera mode:%d\n", mode);

    // cap.set(CV_CAP_PROP_FRAME_WIDTH, 110 ); //不起作用
    // cap.set(CV_CAP_PROP_FRAME_HEIGHT, 100 );

    int fps = cap.get(CV_CAP_PROP_FPS);
    printf("frames per second(fps): %d\n", fps);

    Mat src;
    cap >> src;

    printf("height:%d, width:%d，channels:%d\n", src.cols, src.rows, src.channels());
    
    // klt init  
    unsigned char *img1, *img2;
    KLT_TrackingContext tc;
    KLT_FeatureList fl;
    int nFeatures = 100;
    int ncols, nrows;

    tc = KLTCreateTrackingContext();
    KLTPrintTrackingContext(tc);
    fl = KLTCreateFeatureList(nFeatures);

    Mat src2 = Mat::zeros(src.cols, src.rows, CV_8UC1);
    std::vector<Point2f> p1;
    p1.resize(nFeatures);
    Mat srcTmp;

    while(1)
    {
        double t = (double)getTickCount(); 

        cap >> src;
        if(src.empty())
        {
            printf("can not get mat \n");
            break;
        }   
        src.copyTo(srcTmp);

        cvtColor(src, src, CV_RGB2GRAY);
        img1 = src.data;
        img2 = src2.data;
        nrows = src.rows;
        ncols = src.cols;
        KLTSelectGoodFeatures(tc, img1, ncols, nrows, fl);
        for(int i = 0; i < nFeatures; ++i)
        {
            p1[i].x = fl->feature[i]->x;
            p1[i].y = fl->feature[i]->y;
        }
        KLTTrackFeatures(tc, img1, img2, ncols, nrows, fl);


        for(int i = 0; i < nFeatures; ++i)
        {
            circle(srcTmp, p1[i], 3.f, Scalar(0,255,0));
            if(0 == fl->feature[i]->val)
            {
                line(srcTmp, p1[i], Point2f(fl->feature[i]->x, fl->feature[i]->y), Scalar(0,0,255));
            }
        }
        imshow(windowsName, srcTmp);

        char key = waitKey(1);

        if(key == 27)
        {
            break;
        }
        else if(key == 's')
        {
            waitKey(0);
        }
        swap(src, src2);
    }
    
    cap.release();
    destroyAllWindows();
}

