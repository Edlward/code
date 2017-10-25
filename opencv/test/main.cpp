#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    Mat im = imread("/home/lxg/codedata/0.jpg");
    imshow("src", im);
    waitKey(0);
}