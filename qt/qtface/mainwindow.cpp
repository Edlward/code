#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QImage>
#include <QTimer>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QProcess>

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    camera_on(false),
    database_on(false),
    left_edited(false),
    right_edited(false)
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
    timer->setInterval(500);
    timer->start();

//    timer_show = new QTimer(this); //qtimer need to be included not qtime
//    connect(timer_show, SIGNAL(timeout()), this, SLOT(readAndShowCamera()));
//    timer_show->setInterval(40);
//    timer_show->start();

//    camera_thread = new CameraThread;
//    camera_thread->start();

    face = new Face();

    // read picture
    Mat picture = imread((data_path + "default.jpg").c_str());
    face->picture_face_extract(picture);

    showPicture(picture);
}

MainWindow::~MainWindow()
{
    cap.release();

    delete timer;
    delete ui;
}

void MainWindow::showDatabaseImage(std::string file, std::string name, int win)
{
    std::string image_file(data_path + file);
    cv::Mat im = imread(image_file.c_str());
    if(im.empty())
    {
        QMessageBox msg_box;
        msg_box.setText("database: the image isn't existed");
        msg_box.exec();
    }

    if(win == PICTURE)
    {
        // display picture
        showPicture(im);
        // set txt
        ui->label_name->setText(name.c_str());
    }
}

void MainWindow::showCamera(const Mat &im)
{
    // display camera
    QImage qimg = QImage((const unsigned char*)im.data, im.cols, im.rows, im.cols*im.channels(),
                         QImage::Format_RGB888).rgbSwapped();//rgbSwapped()用于转换QImage对象从RGB->BGR;
    qimg = qimg.scaled(ui->label_camera->width(), ui->label_camera->height(),
                       Qt::KeepAspectRatio, Qt::FastTransformation);
    ui->label_camera->clear();
    ui->label_camera->setPixmap(QPixmap::fromImage(qimg));
}

void MainWindow::showPicture(const Mat &im)
{
    // display picture
    QImage qimg = QImage((const unsigned char*)im.data, im.cols, im.rows, im.cols*im.channels(),
                         QImage::Format_RGB888).rgbSwapped();//rgbSwapped()用于转换QImage对象从RGB->BGR;
    qimg = qimg.scaled(ui->label_picture->width(), ui->label_picture->height(),
                       Qt::KeepAspectRatio, Qt::FastTransformation);
    ui->label_picture->clear();
    ui->label_picture->setPixmap(QPixmap::fromImage(qimg));
}

void MainWindow::readCamera()
{
    if(camera_on)
    {
        return;
    }

    Mat im;
    cap >> im;
    if(im.empty())
    {
        qDebug("video image is empty");
    }
    cv::resize(im, im, Size(0,0), 0.5,0.5);

    double run_time = getTickCount();

    face->camera_face_extract(im);


    float sim = 0;
    if(database_on)
    {
        std::string file;
        std::string name;
        face->searchDb(sim, file, name);
        showDatabaseImage(file, name, PICTURE);
    }
    else
    {
        face->similar_face(sim);
    }

    QString qstr = QString("%1").arg(sim);
    ui->label_sim->setText(qstr);
    run_time = ((double)getTickCount()-run_time)/getTickFrequency();
    qstr = QString("%1").arg(run_time);
    ui->label_fps->setText(qstr);

    showCamera(im);
}

//void MainWindow::readAndShowCamera()
//{
//    if(camera_on)
//    {
//        return ;
//    }

//    Mat im;
//    cap >> im;
//    showCamera(im);
//}

void MainWindow::on_pushButton_clicked()
{
    timer->stop();
    QFileDialog *file_dialog = new QFileDialog(this);
//    QStringList str_list = file_dialog->selectedFiles();
    file_dialog->exec();

    delete file_dialog;

//    QString file_name = QFileDialog::getOpenFileName(this, tr("Open picture"), tr("image(*.jpg)"));
    timer->start();
}

// picture window
void MainWindow::on_openfile_returnPressed()
{
    timer->stop();

    QString qfile_name = ui->openfile->text();
    std::string file_name = qfile_name.toStdString();
    Mat im = imread(file_name.c_str());
    if(im.empty())
    {
        QMessageBox msg_box;
        msg_box.setText("the image isn't existed");
        msg_box.exec();
    }
    else
    {
        face->picture_face_extract(im);
        // display
        showPicture(im);

        // if camera_off checked
        if(camera_on)
        {
            float sim = 0.f;
            face->similar_face(sim);
            QString qstr = QString("%1").arg(sim);
            ui->label_sim->setText(qstr);
        }
    }
    timer->start();
}

// open camera check
void MainWindow::on_check_camera_clicked()
{
    // choosed is true
    camera_on = ui->check_camera->isChecked();
}

// camera right window
void MainWindow::on_openfile_2_returnPressed()
{
    timer->stop();

    QString qfile_name = ui->openfile_2->text();
    std::string file_name = qfile_name.toStdString();
    Mat im = imread(file_name.c_str());
    if(im.empty())
    {
        QMessageBox msg_box;
        msg_box.setText("the image isn't existed");
        msg_box.exec();
    }
    else
    {
        double run_time = getTickCount();

        face->camera_face_extract(im);
        float sim=0.f;

        // if database_on checked
        if(database_on)
        {
            std::string file;
            std::string name;
            face->searchDb(sim, file, name);
            showDatabaseImage(file, name, PICTURE); // show picture on left window(picture)
        }
        else
        {
            face->similar_face(sim);
        }
        run_time = ((double)getTickCount()-run_time)/getTickFrequency();
        QString qstr = QString("%1").arg(run_time);
        ui->label_fps->setText(qstr);
        qstr = QString("%1").arg(sim);
        ui->label_sim->setText(qstr);

        // display
        showCamera(im);
    }
    timer->start();
}

// search database check
void MainWindow::on_check_database_clicked()
{
    database_on = ui->check_database->isChecked();
}

void MainWindow::on_button_detect_clicked()
{
    if(left_edited)
    {
        on_openfile_returnPressed();
        left_edited = false;
    }
    else if(right_edited)
    {
        on_openfile_2_returnPressed();
        right_edited = false;
    }
}

void MainWindow::on_openfile_textEdited(const QString &arg1)
{
    left_edited = true;
}

void MainWindow::on_openfile_2_textEdited(const QString &arg1)
{
    right_edited = true;
}
