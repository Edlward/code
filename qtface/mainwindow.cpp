#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QImage>
#include <QTimer>

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); // this must be the first language

    ui->label_camera->setFixedSize(400,300);
    cap.open(0);
    if(!cap.isOpened())
    {
        qDebug("can not open camera");
    }

    timer = new QTimer(this); //qtimer need to be included not qtime
    connect(timer, SIGNAL(timeout()), this, SLOT(readCamera()));
    timer->start(20);
}

MainWindow::~MainWindow()
{
    cap.release();

    delete timer;
    delete ui;
}

void MainWindow::readCamera()
{
    Mat im;

    cap >> im;
    if(im.empty())
    {
        qDebug("video image is empty");
    }

    // cvtColor(im, im, CV_BGR2RGB);
    QImage qimg = QImage((const unsigned char*)im.data, im.cols, im.rows, im.cols*im.channels(),
                         QImage::Format_RGB888).rgbSwapped();//rgbSwapped()用于转换QImage对象从RGB->BGR;
    qimg = qimg.scaled(ui->label_camera->width(), ui->label_camera->height(),
                       Qt::IgnoreAspectRatio, Qt::FastTransformation);
    ui->label_camera->clear();
    ui->label_camera->setPixmap(QPixmap::fromImage(qimg));

}
