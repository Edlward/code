#ifndef _FACERECOGNIZEHONG_H_
#define _FACERECOGNIZEHONG_H_

#include "faceRecognize.h"
#include "arcsoft_fsdk_face_recognition.h"
#include "merror.h"
#include "faceDetect.h"

class FaceRecognizeHong:public FaceRecognize
{
public:
    FaceRecognizeHong(std::string dbname, FaceDetect *detector);
    ~FaceRecognizeHong();

    bool searchDb(cv::Mat &im);

private:    
    MByte *p_workmem;
    MHandle h_engine;
    FaceDetect *detector;
};

#endif