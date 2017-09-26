#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <cv.h>
#include <iostream>
#include <cstring>
  
using namespace cv;  
using namespace std;  
  
bool selectObject = false;  
Point origin;//用于保存鼠标选择第一次单击时点的位置    
Rect selection;//用于保存鼠标选择的矩形框    
int trackObject = 0;  
  
/*------------------------------- 
 
函数功能 显示轨迹 
 
--------------------------------*/  
void displayTrajectory(Mat img, vector<point>traj, Scalar s)  
{  
    if (!traj.empty())  
    {  
        for (size_t i = 0; i < traj.size() - 1; i++)  
        {  
            line(img, traj[i], traj[i + 1], s, 2, 8, 0);  
        }  
    }  
}  
/*------------------------------- 
 
鼠标控制 
 
--------------------------------*/  
void onMouse(int event, int x, int y, int, void*)  
{  
    if (selectObject)//只有当鼠标左键按下去时才有效，然后通过if里面代码就可以确定所选择的矩形区域selection了    
    {  
        selection.x = MIN(x, origin.x);//矩形左上角顶点坐标    
        selection.y = MIN(y, origin.y);  
        selection.width = std::abs(x - origin.x);//矩形宽    
        selection.height = std::abs(y - origin.y);//矩形高    
  
        selection &= Rect(0, 0, src.cols, src.rows);//用于确保所选的矩形区域在图片范围内  
        //rectangle(src,selection,Scalar(0,0,255),2);  
    }  
    switch (event)  
    {  
    case CV_EVENT_LBUTTONDOWN:  
        origin = Point(x, y);  
        selection = Rect(x, y, 0, 0);//鼠标刚按下去时初始化了一个矩形区域    
        selectObject = true;  
        break;  
    case CV_EVENT_LBUTTONUP:  
        selectObject = false;  
        if (selection.width > 0 && selection.height > 0)  
            trackObject = -1;  
        break;  
    }  
}  
  
int main(int argc, char *argv[])  
{  
    namedWindow("tracker");  
    setMouseCallback("tracker", onMouse(), 0);  
      
    MultiTracker myTracker("KCF");  
    vector<vector<point>> trajectorys;//轨迹记录  
    int objectCounter = 0;  
  
    VideoCapture cap(0);//打开默认的摄像头    
    if (!cap.isOpened())  
    {  
        return -1;  
    }  
    Mat src;  
    bool stop = false;  
    while (!stop)  
    {  
        cap>>src;   
  
        if (selectObject)  
        {  
            rectangle(src, selection, Scalar(0, 0, 255), 2, 8, 0);  
        }  
        if (trackObject < 0)  
        {  
            myTracker.add(src, selection);  
  
            Point cen = Point(selection.x + selection.width / 2.0, selection.y + selection.height / 2.0);  
            trajectorys.resize(objectCounter + 1);  
            trajectorys[objectCounter].push_back(cen);//从[0]开始初始化轨迹起始点  
  
            objectCounter++;//待跟踪目标个数加1  
            trackObject = 1;  
  
        }  
        if (trackObject)  
        {  
            vector<rect2d> r;  
            myTracker.update(src, r);  
  
            size_t s = r.size();  
            RNG rng;  
  
            for (size_t i = 0; i < s; i++)  
            {  
                Scalar scalar = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));  
                rectangle(src, r[i], scalar, 2, 8, 0);  
                Point C = Point(r[i].x + r[i].width / 2.0, r[i].y + r[i].height / 2.0);  
                trajectorys[i].push_back(C);  
                displayTrajectory(src, trajectorys[i], scalar);  
                char name[10];  
                sprintf_s(name, "%d", (i + 1));  
                putText(src, name, r[i].tl(), 3, 0.8, Scalar(0, 255, 255), 2, 8, false);  
            }  
  
        }  
        imshow("tracker", src);  
        if (waitKey(30) == 27) //Esc键退出    
        {  
            stop = true;  
        }  
    }  
    return 0;  
}  
