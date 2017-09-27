#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <forward_list>

using namespace cv;
using namespace std;

class AnObject
{
public:
    AnObject();
    ~AnObject();

    static int edge; //roi中心离边缘超过20像素，则退出

    Ptr<Tracker> tracker;
    Rect2d box_now;
    bool isfound;
    bool out;

    void init(const Mat &im, const Rect2d &roi);
    void update(const Mat &im);    
};

int AnObject::edge = 10;

AnObject::AnObject():isfound(false),out(false)
{
    tracker = TrackerKCF::create();
}

AnObject::~AnObject()
{
    // if(!tracker.empty())
    // {
        tracker.~Ptr();
    // }
}

void AnObject::init(const Mat &im, const Rect2d &roi)
{
    tracker->init(im, roi);
}

void AnObject::update(const Mat &im)
{
    isfound = tracker->update(im, box_now);
}


int main(int argc, char **argv)
{
    forward_list<Ptr<AnObject> > trackers;

    VideoCapture cap;
    cap.open("/home/lxg/livet1.avi");
    if(!cap.isOpened())
    {
        printf("can not open video\n");
        return 0;
    }

    vector<Rect> rois;
    Mat im;
    char key = 0;

    while(1)
    {
        cap >> im;

        if(im.empty())
        {
            printf("video ending\n");
            break;
        }

        switch(key)
        {
        case 's':
        case 'b':
            printf("please select area to track\n");
            selectROIs("tracker", im, rois);
        }
        
        // 添加新目标，新目标和已跟踪目标匹配
        for(size_t i = 0; i < rois.size(); ++i)
        {
            // 已跟踪，则继续下一个
            // 未跟踪，则添加
            Ptr<AnObject> obj;
            obj->init(im, rois[i]);
            trackers.push_front(obj);            
        }

        // 遍历跟踪，更新跟踪
        for(auto iter = trackers.begin(); iter != trackers.end(); ++iter)
        {   
            Ptr<AnObject> obj = iter->get();

            obj->update(im);
            int cx = obj->box_now.x + obj->box_now.width / 2;
            int cy = obj->box_now.y + obj->box_now.height / 2;
            if(cx > im.cols - obj->edge || cx < obj->edge
                || cy < obj->edge || cy > im.rows - obj->edge)
            {
                obj->out = true;
            }
        }

        // 删除已经出视野范围的(out = true) 和 跟踪失败的 (isfound = true)
        trackers.remove_if( [](Ptr<AnObject> &elem) {return elem->out || elem->isfound ;});

        // 目标框显示
        for(auto iter = trackers.begin(); iter != trackers.end(); ++iter)
        {
            Ptr<AnObject> obj = iter->get();
            rectangle(im, obj->box_now, Scalar(0, 0, 255));
        }

        imshow("tracker", im);
        if((key = waitKey(10)) == 27)
        {
            break;
        }
    }
}