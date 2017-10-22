#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

/*************************************************************
 * 从视频抽取图片
 * 视频名及路径 vpath
 * 保存图像路径 ipath
 * 保存图像前缀 prestr
 * 采样频率 sample_fre
**************************************************************/
// string vpath = "/home/lxg/codedata/headXml/india.webm";
string vpath = "/home/lxg/output201709.avi";
string ipath = "/home/lxg/codedata/headXml/srcImage/";
string prestr = "output201709/videoImage/output_01_";
int sample_fre = 2;

int main()
{
    VideoCapture cap;
    // cap.open("/home/lxg/codedata/headXml/output.avi");
    // cap.open("/home/lxg/output.avi");
    cap.open(vpath.c_str());
    if(!cap.isOpened())
    {
        printf("can not open video\n");
    }

    int fps = cap.get(CAP_PROP_FPS);
    int length = cap.get(CAP_PROP_FRAME_COUNT );
    printf("fps:%d \t length:%d\n", fps, length);

    Mat im;
    int interval = 0;
    int num = 0;

    while(1)
    {
        cap >> im;
        if(im.empty())
        {
            printf("video is ending\n");
            break;
        }

        if(interval == sample_fre)
        {
            imwrite((ipath + prestr + to_string(num) + ".jpg").c_str(), im);
            ++num;
            printf("%d\n", num);
            interval = 0;
        }
        ++interval;
    }
}