#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

int main()
{
    ifstream in;
    in.open("/home/lxg/codedata/aflw/data/d.txt");

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
    }

}