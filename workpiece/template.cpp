#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;

void barCallback(int, void *);

Mat resultCopy;
int thresh = 0;
string name = "result";

int main(int argc, char **argv)
{
    Mat src;
    src = imread("test2.jpg");
    float scale = 1.0 * 600 / src.cols;
    resize(src, src, Size(), scale, scale);
    if(!src.data)
    {
        printf("can not open image\n");
        return -1;
    }


    Mat temp;
    temp = imread("backs.png");
    resize(temp, temp, Size(), scale, scale);

    imshow("src", src);
    imshow("template", temp);

    int result_cols =  src.cols - temp.cols + 1;
    int result_rows = src.rows - temp.rows + 1;
    Mat result(result_cols, result_rows, CV_32FC1);

    matchTemplate(src, temp, result, TM_SQDIFF);
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
    result.copyTo(resultCopy);
    imshow("matchResult", result);
    // imshow(name[SRC], src);

    namedWindow(name, WINDOW_NORMAL);
    resizeWindow(name, result.cols, result.rows);
    createTrackbar("thresh", name, &thresh, 255, barCallback);

    waitKey(0);
}

void barCallback(int, void *)
{
    Mat binary;
    float threshT = thresh * 1.0f / 255;

    threshold(resultCopy, binary, threshT, 255, THRESH_BINARY);
    imshow(name, binary);
}