#include <opencv2/opencv.hpp>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

/*
* 扣图程序，同一张图片可以进行多次抠图，
* esc键进行下一张
* space键保存本次抠图框
*/

string path = "/home/lxg/codedata/headXml/";
string name = "src";
string pre_image = "gxl";

int main()
{
    namedWindow(name.c_str(), WINDOW_NORMAL);
    resizeWindow(name.c_str(), 800, 800);

    ifstream in;
    in.open((path+"tmp.txt").c_str());
    if(!in)
    {
        printf("can not open tmp.txt\n");
    }
    
    char str[100];
    vector<string> vname;
    while(in.getline(str, 100))
    {
        vname.push_back(str);
    }

    Mat im;
    Rect roi;
    
    int i = 0;
    // for(; i < 130; ++i)
    // {
    //     in.getline(str, 100);
    // }
    // Mat tmp;
    char key;
    string file;
    vector<Rect> vcrop;
    int num_image = 0;

    for(size_t i = 0; i < vname.size(); ++i)
    {
        file = vname[i];
        im = imread((path + file).c_str());
        resize(im, im, Size(), 3, 3);
        // imshow(name, im);
        vcrop.clear();
        selectROIs(name, im, vcrop);
        for(auto roi:vcrop)
        {
            if(roi.width == 0)
            {
                continue;
            }

            if(roi.x < 0) 
            {
                roi.x = 0;
            }
            if(roi.y < 0)
            {
                roi.y = 0;
            }

            imwrite(path + "bg/bg_15_" + to_string(num_image) + ".jpg" , im(roi));
            // imwrite(path + "head/walmat_2_" + to_string(num_image) + ".jpg" , im(roi));
            
            ++num_image;
            imshow("roi", im(roi));
            key = waitKey(1);
            printf("get %c\n", key);
        }
        printf("%d\n", (int)i);
    }
}