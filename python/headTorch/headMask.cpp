#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <string>
#include <cstdio>

using namespace std;
using namespace cv;

string path = "/home/lxg/codedata/headXml/tensorflow/";

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

int main()
{
    ifstream in;
    in.open((path + "label.txt").c_str());
    if(!in)  cout << "can not open label.txt\n";

    string s1, s2, s3, s4, s5;
    char str[100];
    Mat im, src;
    Mat gauss = Gaussian_kernal(100, 25);

    while(in.getline(str, 100))
    {
        istringstream s(str);
        s >> s1 >> s2 >> s3 >> s4 >> s5;
        
        src = imread((path+"image/"+s1).c_str());
        if(!src.data)
        {
            printf("can not find src image %s\n", s1.c_str());
        }

        im = imread((path+"label/label_"+s1).c_str(), IMREAD_GRAYSCALE);
        if(!im.data)
        {
            // im = Mat::zeros(src.rows, src.cols, CV_8UC1);
            im = Mat::zeros(800, 800, CV_8UC1);
        }

        int x = stoi(s2);
        int y = stoi(s3);
        int w = stoi(s4)-1;
        int h = stoi(s5)-1;
        cout << "x" << x << "\ty" << y << "\tw" << w << "\th" << h << endl;

        Mat gauss_tmp;
        resize(gauss, gauss_tmp, Size(w, h));

        Mat roi(im, Rect(x, y, w, h));
        imshow("roi", roi);
        waitKey(0);

        gauss_tmp.copyTo(roi);
        imwrite((path+"label/label_"+s1).c_str(), im);
    }
}