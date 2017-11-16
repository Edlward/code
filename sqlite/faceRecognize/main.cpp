#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    VideoCapture cap;
    cap.open(0);
    if(!cap.isOpened())
    {
        cout << "can not open failure\n";
        return -1;
    }

    Mat im;
    while(true)
    {
        cap >> im;
        
    }

}