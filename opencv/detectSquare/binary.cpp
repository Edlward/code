#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    VideoCapture cap;
    cap.open("/home/lxg/codedata/square/s1.avi");
    cout << "height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;

    Mat im, roi, gray, dst;
    Rect2d rect_roi(300, 180, 600, 500);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    char key = 0;

    while(1)
    {
        cap >> im;
        cvtColor(im, gray, CV_RGB2GRAY);
        roi = gray(rect_roi);
        rectangle(im, rect_roi, Scalar(0,0,255), 2);

        threshold(roi, dst, 70, 255, THRESH_OTSU);
        imshow("binary", dst);
        imshow("square", im);
        if((key = waitKey(10)) == 27)
        {
            break;
        }
        else if(key == 's')
        {
            waitKey(0);
        }

    }
}