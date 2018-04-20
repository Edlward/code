#include <fstream>
#include <opencv2/opencv.hpp>
#include <cstdio>

using namespace cv;
using namespace std;

string root_path = "/home/lxg/codedata/aflw/";
string file[3] = {"data/0/", "data/2/", "data/3/"};

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


bool readImage(Mat &src, const string &name, string &path)
{
    src = imread((root_path + file[0] + name).c_str());
    if(!src.empty())
    {
        path = "0/";
        return true;
    }

    src = imread((root_path + file[1] + name).c_str());
    if(!src.empty())
    {
        path = "2/";
        return true;
    }

    src = imread((root_path + file[2] + name).c_str());
    if(!src.empty())
    {
        path = "3/";
        return true;
    }

    return false;
}

int main(int argc, char **argv)
{
    ofstream errlog;
    errlog.open("/home/lxg/codedata/aflw/data/errlog.txt");

    ifstream in;
    in.open("/home/lxg/codedata/aflw/data/d.txt");
    
    // kernel_size, sigma
    Mat gaussian_kernel = Gaussian_kernal(101, 30);
    // imshow("gauss", gaussian_kernel);
    cout << gaussian_kernel << endl;
    // waitKey(0);

    Mat gaussian_kernel_tmp;

    char str[100];
    string s1, s2, s3, s4, s5, s6, s7, s8, s9;
    string file_path;

    in.getline(str, 100); // 舍弃第一行
    cout << "first line " << str << endl;
    
    int i = 0;
    Mat src;
    int w, h, x, y;

    while(in.getline(str, 100))
    {
        cout << str << endl;

        istringstream s(str);
        s >> s1 >> s2 >> s3 >> s4 >> s5 >> s6 >> s7 >> s8 >> s9;

        // cout << "s1 " << s1 << endl;
        // cout << "s2 " << s2 << endl;
        // cout << "s3 " << s3 << endl;
        // cout << "s4 " << s4 << endl;
        // cout << "s5 " << s5 << endl;
        // cout << "s6 " << s6 << endl;
        // cout << "s7 " << s7 << endl;
        // cout << "s8 " << s8 << endl;
        // cout << "s9 " << s9 << endl;

        // errlog << "err test " << endl;
        if(!readImage(src, s8, file_path))
        {
            errlog << "can not open image: " << s8 << endl;            
            continue;
        }

        x = stoi(s3);
        y = stoi(s4);
        w = stoi(s5);
        h = stoi(s6);

        x = std::max(x, 0);
        y = std::max(y, 0);
        w = std::min(w, src.cols - x);
        h = std::min(h, src.rows - y);

        
        resize(gaussian_kernel, gaussian_kernel_tmp, Size(w, h));
        
        Mat src_label1, src_label2;
        src_label1 = Mat::zeros(src.rows, src.cols, CV_8UC1);
        
        Mat roi(src_label1, Rect(x, y, w, h));
        gaussian_kernel_tmp.copyTo(roi);

        src_label2 = imread((root_path + "label/" + file_path + "label_" + s8).c_str(), IMREAD_GRAYSCALE);
        if(!src_label2.empty())
        {
            errlog << "one image two more faces " << s8 << endl;
            src_label1 = src_label2 + src_label1;
        }
        
        
        imwrite((root_path + "label/" + file_path + "label_" + s8).c_str(), src_label1);
        
        // ++i;
        // if(i == 10)
        // {
        //     break;
        // }
    }
    errlog.close();
    in.close();
}