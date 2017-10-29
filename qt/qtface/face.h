#ifndef FACE_H
#define FACE_H

#include "opencv2/opencv.hpp"
#include "face_detection.h"
#include "face_alignment.h"
#include "face_identification.h"
#include "database.h"

class Face
{
public:
    Face();

    void camera_face_extract(cv::Mat &img);
    void picture_face_extract(cv::Mat &img);
    void similar_face(float &sim);

    seeta::FaceDetection detector;
    seeta::FaceAlignment point_detector;
    seeta::FaceIdentification face_recognizer;
    seeta::ImageData img_data;

    Database db;
    void searchDb(float &sim, std::string &file, std::string &str_name);
private:
    bool picture_flg;
    float picture_fea[2048];
    std::vector<seeta::FaceInfo> picture_info;
    seeta::FacialLandmark picture_points[5];

    bool camera_flg;
    float camera_fea[2048];
    std::vector<seeta::FaceInfo> camera_info;
    seeta::FacialLandmark camera_points[5];


    void detectface(std::vector<seeta::FaceInfo> &faces);
    void alignmentface(const seeta::FaceInfo face, seeta::FacialLandmark *points);
    void identiface(const cv::Mat &img, seeta::FacialLandmark *points, float *feature);

};

#endif // FACE_H
