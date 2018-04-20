#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

string path = "/home/lxg/codedata/";

Mat Gaussian_kernal(int kernel_size, int sigma)
{
    const double PI = 3.14159265358979323846;
    int m = kernel_size / 2;
    Mat kernel(kernel_size, kernel_size, CV_32FC1);
    float s = 2 * sigma*sigma;
    for (int i = 0; i < kernel_size; i++)
    {
        for (int j = 0; j < kernel_size; j++)
        {
            int x = i - m, y=j - m;
            kernel.ptr<float>(i)[j] = exp(-(x*x + y*y) / s) / (PI*s);
        }
    }

    double min, max;
    minMaxLoc(kernel, &min, &max);
    kernel = kernel / max * 255;
    
    Mat kernel_8uc1;
    kernel.convertTo(kernel_8uc1, CV_8UC1);

    return kernel_8uc1;
}

// int main()
// {
//     // sigma越大越分散
//     Mat gaus = Gaussian_kernal(101, 30);
//     // std::cout << gaus << endl;
//     imshow("kernel", gaus);

//     // double min, max;
//     // Point a, b;
//     // minMaxLoc(gaus, &min, &max, &a, &b);

//     // cout << "min" << min << endl;
//     // cout << "max" << max << endl;

//     // gaus = gaus / max;

//     // imshow("unify-gauss", gaus);

//     // gaus = gaus * 255;

//     // Mat igaus;
//     // gaus.assignTo(igaus, CV_8UC1);

//     // imshow("gaus255", igaus);

//     Mat im2 = imread("test.jpg", IMREAD_GRAYSCALE);
//     Mat im3 = Mat::zeros(200, 200, CV_8UC1);
//     cout << "width " << im2.cols << " height " << im2.rows << " channel " << im2.channels() << endl;
    
//     // Mat roi2(im2, Rect(0,0,101,101));
//     // gaus.copyTo(roi2);
//     // imshow("label", im2);
//     imshow("src", im2);
//     Mat roi3(im2, Rect(50,50,101,101));
//     cout << "width " << roi3.cols << " height " << roi3.rows << " channel " << roi3.channels() << endl;
    
//     gaus.copyTo(roi3);
//     imshow("label2", im2);

//     cout << "width " << im2.cols << " height " << im2.rows << " channel " << im2.channels() << endl;
    
//     imwrite("test2.jpg", im2);
//     Mat im4 = (im2 + im3) / 2;
//     imshow("im4", im4);
//     // Mat gauss = Mat::ones(1, 1, CV_8UC1)*255;
//     // Mat kernel;
    
//     // std::cout << "gauss" << gauss;

//     // GaussianBlur(gauss, kernel, Size(101,101), 1, 1, BORDER_ISOLATED);
//     // std::cout << "kernel" << kernel;

//     // imshow("kernel", kernel);
    
//     // Mat roi2(im2, Rect(0,0,100,100));
//     // imshow("src", im2);
//     // kernel.copyTo(roi2);
//     // imshow("copy", im2);

//     waitKey(0);
// }

// 头部负样本
int main()
{
    // sigma越大越分散
    Mat gaus = Gaussian_kernal(24, 7);
    // std::cout << gaus << endl;
    // resize(gaus, gaus, Size(500,500));
    imshow("kernel", gaus);
    imwrite("/home/lxg/codedata/posLabel.jpg", gaus);
    // Mat im2 = imread("test.jpg", IMREAD_GRAYSCALE);
    // Mat im3 = Mat::zeros(200, 200, CV_8UC1);
    // cout << "width " << im2.cols << " height " << im2.rows << " channel " << im2.channels() << endl;
    
    // // Mat roi2(im2, Rect(0,0,101,101));
    // // gaus.copyTo(roi2);
    // // imshow("label", im2);
    // imshow("src", im2);
    // Mat roi3(im2, Rect(50,50,101,101));
    // cout << "width " << roi3.cols << " height " << roi3.rows << " channel " << roi3.channels() << endl;
    
    // gaus.copyTo(roi3);
    // imshow("label2", im2);

    // cout << "width " << im2.cols << " height " << im2.rows << " channel " << im2.channels() << endl;
    
    // imwrite("test2.jpg", im2);
    // Mat im4 = (im2 + im3) / 2;
    // imshow("im4", im4);
    // Mat gauss = Mat::ones(1, 1, CV_8UC1)*255;
    // Mat kernel;
    
    // std::cout << "gauss" << gauss;

    // GaussianBlur(gauss, kernel, Size(101,101), 1, 1, BORDER_ISOLATED);
    // std::cout << "kernel" << kernel;

    // imshow("kernel", kernel);
    
    // Mat roi2(im2, Rect(0,0,100,100));
    // imshow("src", im2);
    // kernel.copyTo(roi2);
    // imshow("copy", im2);

    waitKey(0);
}