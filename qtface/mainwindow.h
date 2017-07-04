#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QTime>
#include <string>
#include "opencv2/opencv.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    cv::Mat src;
    cv::VideoCapture cap;



private slots:
    void readCamera();

private:
    Ui::MainWindow *ui;
    QTimer* timer;



};

const std::string datapath = "/home/lxg/codedata/";

#endif // MAINWINDOW_H
