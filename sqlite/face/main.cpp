#include "face.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    VideoCapture cap;
    cap.open(0);
    if(!cap.isOpened())
    {
        std::cout << "can not open camera\n";
    }

    int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    cout << "width:" << width << "\theight:" << height << endl;

    string db_file("/home/lxg/Qt5.8.0/mycode/build-test-Desktop_Qt_5_8_0_GCC_64bit-Debug/faceData.db");
    
    Face face(db_file, width, height);
    Mat im;

    char key;
    while(true)
    {
        cap >> im;
        face.detectAndRecognize(im);
        face.showFace(im);
        key = waitKey(20);
        if(key == 27)
        {
            break;
        }
    }
}