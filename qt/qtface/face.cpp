#include "face.h"
#include "mainwindow.h"
#include <QDebug>
#include <cstring>

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

        rectangle(img, face_rect, Scalar(0,0,255),2); // the color channel in opencv is bgr
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

void Face::searchDb(float &sim, std::string &file, std::string &str_name)
{
    std::string sql = "select * from facefeature";
    int rc;
    rc = sqlite3_prepare(db.db_feature, sql.c_str(), sql.length(), &db.stmt, NULL);
    if(rc != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db.db_feature);
    }

    rc = 0;
    float sim_tmp = 0.f;
    float sim_max = 0.f;
    int id_max = 1;
    int id = 1;

    while(1)
    {
        // SQLITE_ROW sqlite_step() has another row already
        // SQLITE_DONE sqlite_step() has finished executing
        rc = sqlite3_step(db.stmt);
        if(rc == SQLITE_DONE)
        {
            break;
        }

        id = sqlite3_column_int(db.stmt, 0);
        // qDebug() << "id:" << id;


        float *fea_tmp = (float *)sqlite3_column_blob(db.stmt, 2);
        for(int i = 0; i < 2048; ++i)
        {
            picture_fea[i] = fea_tmp[i];
        }
        // std::memcpy(camera_fea, fea_tmp, 2048*sizeof(float));

        // compute similarity of camera and picture
        similar_face(sim_tmp);
        if(sim_tmp > sim_max)
        {
            sim_max = sim_tmp;
            id_max = id;
        }
    }

    char sql_max[50];
    std::sprintf(sql_max, "select * from persons where id = %d", id_max);
    rc = sqlite3_prepare(db.db_person, sql_max, std::strlen(sql_max), &db.stmt, 0);
    if(rc != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db.db_person);
    }
    sqlite3_step(db.stmt);

    char *image = (char *)sqlite3_column_text(db.stmt, 1);
    char *name = (char *)sqlite3_column_text(db.stmt, 2);
    int age = sqlite3_column_int(db.stmt, 3);

    qDebug() << image;
    qDebug() <<  name;

    if(image != NULL)
    {
        file = image;
    }
    if(name != NULL)
    {
        str_name = name;
    }
    sim = sim_max;
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

