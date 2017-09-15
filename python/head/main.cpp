#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace cv::dnn;

#include <iostream>

int main()
{
    String model_file = "/home/lxg/codedata/head/head2.pkl";
    String image_file = "/home/lxg/codedata/head/";

    dnn::Net net = dnn::readNetFromTorch(model_file);

}