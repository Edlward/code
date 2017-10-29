#include "face.h"
#include "mainwindow.h"

using namespace cv;


Face::Face():detector((model_path+"seeta_fd_frontal_v1.0.bin").c_str()),
            point_detector((model_path+"seeta_fa_v1.1.bin").c_str()),
            face_recognizer((model_path+"seeta_fr_v1.0.bin").c_str())
{
    detector.SetMinFaceSize(40);
    detector.SetScoreThresh(2.f);
    detector.SetImagePyramidScaleFactor(0.8f);
    detector.SetWindowStep(4, 4);
}

void Face::camera_face_extract(Mat &img)
{
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_RGB2GRAY);

    img_data.data = img_gray.data;
    img_data.width = img_gray.cols;
    img_data.height = img_gray.rows;
    img_data.num_channels = img_gray.channels();
    printf("load image test\n");

    detectface(camera_info);
    if(camera_info.empty())
    {
        camera_flg = false;
        return;
    }
    camera_flg = true;
    alignmentface(camera_info[0], camera_points);
    identiface(img, camera_points, camera_fea);

    for(unsigned int i = 0; i < camera_info.size(); ++i)
    {
        cv::Rect face_rect;
        face_rect.x = camera_info[i].bbox.x;
        face_rect.y = camera_info[i].bbox.y;
        face_rect.width = camera_info[i].bbox.width;
        face_rect.height = camera_info[i].bbox.height;

        rectangle(img, face_rect, Scalar(0,0,255),2);
    }

    int pts_num = 5;
    for(int i=0; i < pts_num; ++i)
    {
        circle(img, Point(camera_points[i].x, camera_points[i].y), 2, Scalar(0,255,0), CV_FILLED);
    }

}

void Face::picture_face_extract(Mat &img)
{
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_RGB2GRAY);

    img_data.data = img_gray.data;
    img_data.width = img_gray.cols;
    img_data.height = img_gray.rows;
    img_data.num_channels = img_gray.channels();
    printf("load image test\n");

    detectface(picture_info);
    if(picture_info.empty())
    {
        picture_flg = false;
        return;
    }
    picture_flg = true;
    alignmentface(picture_info[0], picture_points);
    identiface(img, picture_points, picture_fea);
    for(unsigned int i = 0; i < picture_info.size(); ++i)
    {
        cv::Rect face_rect;
        face_rect.x = picture_info[i].bbox.x;
        face_rect.y = picture_info[i].bbox.y;
        face_rect.width = picture_info[i].bbox.width;
        face_rect.height = picture_info[i].bbox.height;

        rectangle(img, face_rect, Scalar(0,0,255),2);
    }

    int pts_num = 5;
    for(int i=0; i < pts_num; ++i)
    {
        circle(img, Point(picture_points[i].x, picture_points[i].y), 2, Scalar(0,255,0), CV_FILLED);
    }
}

void Face::similar_face(float &sim)
{
    if(picture_flg && camera_flg)
    {
        sim = face_recognizer.CalcSimilarity(camera_fea, picture_fea);
    }
}


void Face::detectface(std::vector<seeta::FaceInfo> &faces)
{
    long t0 = cv::getTickCount();
    printf("detect start\n");
    faces = detector.Detect(img_data);
    printf("detect done\n");
    long t1 = cv::getTickCount();
    double secs = (t1 - t0)/cv::getTickFrequency();
}

void Face::alignmentface(const seeta::FaceInfo face, seeta::FacialLandmark *points)
{
    point_detector.PointDetectLandmarks(img_data, face, points);
}

void Face::identiface(const Mat &img, seeta::FacialLandmark *points, float *feature)
{
    seeta::ImageData color;
    color.data = img.data;
    color.width = img.cols;
    color.height = img.rows;
    color.num_channels = img.channels();

    face_recognizer.ExtractFeatureWithCrop(color, points, feature);
}

