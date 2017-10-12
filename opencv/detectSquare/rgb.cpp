#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

Point2d mouse_p;
void mouseHandler(int event, int x, int y, int, void *param)
{
    if(event == CV_EVENT_LBUTTONDOWN)
    {
        mouse_p.x = x;
        mouse_p.y = y;
        // cout << 
        Mat *im = (Mat *)param;
        cout << "x,y" << x << " " << y << "\t" << im->at<Vec3b>(y,x) << endl;
    }
}

int main(int argc, char **argv)
{
    namedWindow("src", WINDOW_NORMAL);
    namedWindow("luv");

    VideoCapture cap;
    cap.open("/home/lxg/codedata/square/s1.avi");
    cout << "height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;

    // vector<Mat> split_rgb;
    Mat temp = imread("/home/lxg/codedata/square/temp.png");
    if(temp.empty())
    {
        cout << "can not open image\n";
        return 0;
    }
    Scalar rgb = sum(temp) / (temp.cols * temp.rows);
    cout << rgb << endl;


    Mat im, roi, gray, binary, luv, yuv;

    Rect2d rect_roi(600, 180, 600, 250);
    // Rect2d rect_roi(0, 30, 700, 250);  //s2
    
    // Rect2d rect_roi(300, 180, 600, 400);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    int thresh = 50;
    char key = 0;

    while(1)
    {
        cap >> im;
        cvtColor(im, gray, CV_RGB2GRAY);
        cvtColor(im, luv, CV_RGB2Luv);
        // cout << luv;
        setMouseCallback("luv", mouseHandler, &luv);
        // inRange()
        roi = im(rect_roi);
        inRange(roi, Scalar(rgb[0]-thresh, rgb[1]-thresh, rgb[2]-thresh), Scalar(rgb[0]+thresh, rgb[1]+thresh, rgb[2]+thresh), binary);
        
        // cvtColor(im, yuv, CV_BGR2YUV);
        // roi = yuv(rect_roi);
        // // inRange(roi, Scalar(0, 129, 114), Scalar(255, 139, 124), binary);
        // inRange(roi, Scalar(0, 128, 112), Scalar(255, 140, 126), binary);

        // cvtColor(im, yuv, CV_BGR2Luv);
        // roi = yuv(rect_roi);
        // inRange(roi, Scalar(40, 86, ));

        // cvtColor(im, yuv, CV_BGR2Lab);
        // roi = yuv(rect_roi);
        // inRange(roi, Scalar(47, 122, 111), Scalar(198, 128, 125), binary);

        cvtColor(im, yuv, CV_BGR2Luv);
        roi = yuv(rect_roi);
        inRange(roi, Scalar(22, 84, 109), Scalar(223, 96, 137), binary);
        
        rectangle(im, rect_roi, Scalar(0,0,255));
        imshow("binary", binary);
        imshow("im", im);
        // imshow("luv", luv);

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