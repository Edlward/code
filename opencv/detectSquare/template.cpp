#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

string path = "/home/lxg/codedata/square/";

int main(int argc, char **argv)
{
    VideoCapture cap;
    cap.open("/home/lxg/codedata/square/s1.avi");
    cout << "height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;

    Mat im, roi, gray;
    // Rect2d rect_roi(600, 180, 600, 250);
    Rect2d rect_roi(300, 180, 600, 500); // track
    

    Mat temp, temp_gray, result;
    // temp = imread("/home/lxg/codedata/square/temp.png");
    // cvtColor(temp, temp_gray, CV_RGB2GRAY);
    
    Mat temp5, temp2, temp3, temp4;
    temp2 = imread((path+"Selection_002.png").c_str());
    temp3 = imread((path+"Selection_003.png").c_str());
    temp4 = imread((path+"Selection_004.png").c_str());
    temp5 = imread((path+"Selection_005.png").c_str());
    resize(temp2, temp2, Size(100,100));
    resize(temp3, temp3, Size(100,100));
    resize(temp4, temp4, Size(100,100));
    resize(temp5, temp5, Size(100,100));
    // temp = temp2/3 + temp3/3 + temp4/3;// + temp5/4;
    temp = (temp2 + temp3)/2;// + temp4) / 3;// + temp5/4;
    
    cvtColor(temp, temp_gray, CV_RGB2GRAY);

    imshow("temp", temp_gray);

    while(1)
    {
        cap >> im;
        cvtColor(im, gray, CV_RGB2GRAY);
        roi = gray(rect_roi);
        rectangle(im, rect_roi, Scalar(0,0,255), 2);

        // matchTemplate(roi, temp_gray, result, TM_CCORR_NORMED ); //可以进一步尝试
        // matchTemplate(roi, temp_gray, result, TM_CCOEFF ); //很奇怪，像是二值化了的
        matchTemplate(roi, temp_gray, result, TM_CCOEFF_NORMED);

        imshow("roi", roi);

        imshow("square", result);
        if(waitKey(10) == 27)
        {
            break;
        }

    }
}