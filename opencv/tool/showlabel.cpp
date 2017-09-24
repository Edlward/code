#include <opencv2/opencv.hpp>
#include <fstream>
#include <string>
#include <cstdio>

using namespace cv;
using namespace std;

string path = "/home/lxg/codedata/headXml/srcImage/walmat/";

int main()
{
    ifstream in;
    in.open((path+"label_walmat.txt").c_str());
    if(!in)
    {
        cout << "can not open label.txt\n";
    }

    char str[100];
    Mat im;
    string s1, s2, s3, s4, s5;
    char key;
    while(in.getline(str, 100))
    {
        istringstream s(str);
        s >> s1 >> s2 >> s3 >> s4 >> s5;

        int x = stoi(s2);
        int y = stoi(s3);
        int w = stoi(s4);
        int h = stoi(s5);

        im = imread((path+s1).c_str());
        if(im.empty())
        {
            cout << "can not open picture " << s1 << endl;
            break;
        }
        rectangle(im, Rect(x,y,w,h), Scalar(0,0,255));
        imshow("src_label", im);
        
        key = waitKey(0);
        if(key == 27)
        {
            break;
        }
    }
}