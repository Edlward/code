#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QTime>
#include <string>
#include <opencv2/opencv.hpp>
#include "face.h"
#include "camerathread.h"

namespace Ui {
class MainWindow;
}

enum WINDOWS
{
    CAMERA,
    PICTURE
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    cv::Mat src;
//    cv::Mat picture;
    cv::VideoCapture cap;

    void showDatabaseImage(std::string file, std::string name, int win);

    void showCamera(const cv::Mat &im);
    void showPicture(const cv::Mat &im);


private slots:
    void readCamera();

//    void readAndShowCamera();

    void on_pushButton_clicked();

    void on_openfile_returnPressed();

    void on_check_camera_clicked();

    void on_openfile_2_returnPressed();

    void on_check_database_clicked();

    void on_button_detect_clicked();

    void on_openfile_textEdited(const QString &arg1);

    void on_openfile_2_textEdited(const QString &arg1);

private:
    bool camera_on;
    bool database_on;
    bool left_edited;
    bool right_edited;

    Ui::MainWindow *ui;
    QTimer* timer;
//    QTimer* timer_show;
    CameraThread *camera_thread;
    Face *face;
};

const std::string data_path = "/home/lxg/codedata/face/picture/";
const std::string model_path = "/home/lxg/codedata/face/model/";
const std::string db_path = "/home/lxg/codedata/face/";

#endif // MAINWINDOW_H
