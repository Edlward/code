#ifndef _FACEDETECTHONG_H_
#define _FACEDETECTHONG_H_

#include <opencv2/opencv.hpp>
#include "faceDetect.h"
#include "arcsoft_fsdk_face_detection.h"
#include "merror.h"
#include "faceRecognizeHong.h"

class FaceDetectHong: public FaceDetect
{
public:
    FaceDetectHong(int w, int h);
    ~FaceDetectHong();

    bool detectFace(const cv::Mat &im);
    // friend class FaceRecognizeHong;

private:
    
    MByte *p_workmem;
    MHandle h_engine;
    ASVLOFFSCREEN input_img;
    int image_buf_len;
    int image_width;
    int image_height;
};

#endif
