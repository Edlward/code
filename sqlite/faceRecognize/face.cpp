#include "face.h"
#include "faceDetectHong.h"
#include "faceRecognizeHong.h"

Face::Face(std::string dbname, int w, int h)
{
    detector = new FaceDetectHong(w, h);
    recognizer = new FaceRecognizeHong(dbname, detector);
    
}

Face::~Face()
{
    delete recognizer;
    delete detector;
}

bool Face::detectAndRecognize(cv::Mat &im)
{
    detector->detectFace(im);
    
    for(size_t i = 0; i < detector->face_roi.size(); ++i)
    {
        // recognizer->
    }
}

