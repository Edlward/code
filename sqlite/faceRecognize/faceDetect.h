#ifndef _FACEDETECT_H_
#define _FACEDETECT_H_

#include <opencv2/opencv.hpp>
#include <vector>
class FaceDetect
{
public:
    FaceDetect(int w, int h)
    {
        this->image_width = w;
        this->image_height = h;
    }

    // ~FaceDetect();
    
    // virtual bool detectorInit()=0;
    virtual bool detectFace(const cv::Mat &im)
    {
        std::cout << "abstract base class has been called" << std::endl;
    }
    
    int image_width;
    int image_height;

    std::vector<cv::Rect> face_roi;
private:    


};


#endif