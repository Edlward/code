#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

int main()
{
    VideoCapture cap;
    // cap.open("/home/lxg/codedata/headXml/output.avi");
    // cap.open("/home/lxg/output.avi");
    cap.open("/home/lxg/codedata/headXml/india.webm");
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
    string path = "/home/lxg/codedata/headXml/";

    while(1)
    {
        cap >> im;
        if(im.empty())
        {
            printf("video is ending\n");
            break;
        }

        if(interval == 5)
        {
            imwrite((path + "videoImage/india_0_" + to_string(num) + ".jpg").c_str(), im);
            ++num;
            printf("%d\n", num);
            interval = 0;
        }
        ++interval;
    }
}