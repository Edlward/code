#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    VideoCapture cap;
    cap.open("/home/lxg/codedata/square/s2.avi");
    cout << "height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;

    Mat im, roi, gray, binary;
    // Rect2d rect_roi(600, 180, 600, 250); //s1
    Rect2d rect_roi(0, 30, 700, 250);  //s2
    // Rect2d rect_roi(600, 180, 600, 250); //s4
    

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    // Mat element = getStructuringElement(element_shape, Size(an*2+1, an*2+1), Point(an, an) );
    Mat edge_im = Mat::zeros(rect_roi.height, rect_roi.width, CV_8UC3);
    
    while(1)
    {
        cap >> im;
        cvtColor(im, gray, CV_RGB2GRAY);
        rectangle(im, rect_roi, Scalar(0,0,255), 2);

        roi = gray(rect_roi);
        // canny
        Canny(roi, binary, 90, 100); 
        dilate(binary, binary, 0, Point(-1,-1));
        
        
        // threshold(roi, binary, 0, 255, THRESH_OTSU);
        // imshow("binary", binary);
        // findContours(binary, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
        // for(size_t i = 0; i < contours.size(); ++i)
        // {
        //     if(contours[i].size() < 100)
        //     {
        //         continue;
        //     }
        //     Scalar color(rand()&255, rand()&255, rand()&255);
        //     // drawContours(im(rect_roi), contours, i, color, 2, 8);
        //     drawContours(edge_im, contours, i, color, 2, 8);
        // }

        imshow("edge_im", edge_im);
        imshow("binary", binary);
        imshow("square", im);
        if(waitKey(1) == 27)
        {
            break;
        }

    }
}