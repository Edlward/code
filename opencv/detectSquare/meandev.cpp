#include <opencv2/opencv.hpp>
#include <getopt.h>
#include <cmath>

using namespace cv;
using namespace std;

void computeAndPrint(string name, Mat im)
{
    Scalar mean, stddev;
    meanStdDev(im, mean, stddev);
    cout << name << mean << stddev << (mean - 2*stddev) << (mean + 2*stddev) << endl;
}
int main(int argc, char **argv)
{
    char ch;
    char *img;

    while((ch = getopt(argc, argv, "i:")) != -1)
    {
        switch(ch)
        {
        case 'i':
            img = optarg;
            cout << "img: " << optarg << endl;
            
        }
    }
    string path = "/home/lxg/codedata/square/";
    
    vector<string> file;
    file.push_back("Selection_013.png");
    file.push_back("Selection_014.png");
    file.push_back("Selection_015.png");
    file.push_back("Selection_016.png");
    file.push_back("Selection_017.png");
    file.push_back("Selection_018.png");
    file.push_back("Selection_019.png");
    file.push_back("Selection_020.png");
    file.push_back("Selection_021.png");
    
    // vector<Scalar> vmean;
    // vector<Scalar> vdev;

    // for(size_t i = 0; i < file.size(); ++i)
    // {
    //     Mat im = imread((path+file[i]).c_str());
    //     if(im.empty())
    //     {
    //         cout << "can not open image\n";
    //     }

    //     Mat luv;
    //     cvtColor(im, luv, CV_BGR2Lab);
        
    //     Scalar mean, dev;
    //     meanStdDev(luv ,mean, dev);
    //     cout << "mean " << mean << endl;
    //     cout << "dev " << dev << endl;
    //     vmean.push_back(mean);
    //     vdev.push_back(dev);
    // }

    // // compute mean
    // Scalar smean;
    // for(size_t i = 0; i < vmean.size(); ++i)
    // {
    //     smean += vmean[i] * (1.0 / vmean.size());
    // }

    // // compute dev
    // Scalar sdev;
    // for(size_t i = 0; i < vdev.size(); ++i)
    // {
    //     sdev += (vmean[i] - smean) * (vmean[i] - smean);
    // }
    // sdev = sdev * (1.0 / vmean.size());
    // cout << "smean " << smean << endl;
    // cout << "sdev " << sdev << endl;

    Mat simg = Mat::zeros(50*file.size(), 50, CV_8UC3);
    Rect rect_roi;
    Mat tmp, roi;
    for(size_t i = 0; i < file.size(); ++i)
    {
        rect_roi = Rect(0,i*50,50,50);
        tmp = imread((path+file[i]).c_str());
        resize(tmp, tmp, Size(50,50));
        roi = simg(rect_roi);
        tmp.copyTo(roi);
    }

    Mat luv;
    Scalar mean, dev;
    
    cvtColor(simg, luv, CV_BGR2RGB);    //dev [49.5828, 53.7049, 58.6522, 0]    
    computeAndPrint("rgb", luv);

    cvtColor(simg, luv, CV_BGR2Luv);    //dev [55.1358, 2.84664, 7.22481, 0]  mean [100.489, 91.7038, 126.656, 0]    
    computeAndPrint("luv", luv);
    
    cvtColor(simg, luv, CV_BGR2Lab);    //dev [55.1731, 1.69713, 4.75939, 0]    mean [100.581, 125.884, 120.398, 0]    
    computeAndPrint("lab", luv);
    
    cvtColor(simg, luv, CV_BGR2HSV);    //dev [9.10554, 22.3345, 58.6306, 0]
    computeAndPrint("hsv", luv);
    
    cvtColor(simg, luv, CV_BGR2HLS);    //dev [9.10383, 53.8956, 24.8437, 0]
    computeAndPrint("hls", luv);
    
    cvtColor(simg, luv, CV_BGR2YUV);    //dev [52.9203, 4.43766, 4.93131, 0]  mean [94.352, 134.441, 119.16, 0]
    computeAndPrint("yuv", luv);

    imshow("simg", simg);
    imshow("tmp", tmp);
    waitKey(0);    
}