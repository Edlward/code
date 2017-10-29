#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <string>
#include "database.h"
#include "face.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(!QSqlDatabase::drivers().contains("QSQLITE"))
    {
        QMessageBox::critical(this, "unable to load database", "the demo needs the sqlite driver");
    }

    QSqlError err = pdb.getConnection();
    if(err.type() != QSqlError::NoError)
    {
        showError(err);
    }

    model = new QSqlTableModel(this);
    model->setTable("persons");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select(); // get all rows??

    ui->tableView->setModel(model);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_add_clicked()
{
    int row_num = model->rowCount();
    QString str = ".jpg";

    model->insertRow(row_num);
    model->setData(model->index(row_num,1), str);
    model->setData(model->index(row_num,0), row_num+1);
    model->setData(model->index(row_num, 5), false);
}

void MainWindow::on_button_submit_clicked()
{
    model->database().transaction();
    if(model->submitAll())
    {
        model->database().commit();
    }
    else
    {
        model->database().rollback();
        QMessageBox::warning(this, tr("tableModel"), tr("database err: %1").arg(model->lastError().text()));
    }
}

// show picture and add feature
void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    qDebug() << "clicked" << index;

    if(index.column() == 1)
    {
        QVariant image = model->data(index);
        qDebug() << image;

        std::string img_file = path_image.toStdString();
        img_file += image.toString().toStdString();

        cv::Mat picture;
        picture = cv::imread(img_file.c_str());

        ui->label_picture->clear();

        if(picture.empty())
        {
            qDebug() << "can not load image";
            qDebug() << img_file.c_str();
            model->setData(model->index(index.row(), 5), false);
            return;
        }

        QImage qimg = QImage((const unsigned char *)picture.data, picture.cols, picture.rows,
                             picture.cols*picture.channels(), QImage::Format_RGB888).rgbSwapped();
        qimg = qimg.scaled(ui->label_picture->width(), ui->label_picture->height(),
                           Qt::IgnoreAspectRatio, Qt::FastTransformation);
        ui->label_picture->setPixmap(QPixmap::fromImage(qimg));

        // check feature already been set
        QModelIndex index_tmp;
        index_tmp = index.sibling(index.row(), 5);
        // qDebug() << index_tmp;
        if(model->data(index_tmp).toBool())
        {
             return;
        }

        // seetaface
        Face face;
        face.camera_face_extract(picture);
        if(face.camera_info.size() == 0)
        {
            qDebug() << "zero face has been found";
            return ;
        }
        else
        {
            qDebug() << "found face:" << face.camera_info.size();
        }

        qDebug() << face.camera_fea[0];
        qDebug() << face.camera_fea[1];
        qDebug() << face.camera_fea[2];

        index_tmp = index.sibling(index.row(), 0);
        // qDebug() << index_tmp;

        int id = model->data(index_tmp).toInt();
        index_tmp = index.sibling(index.row(), 1);
        QString name = model->data(index_tmp).toString();

        // qDebug() << "qt:" << id;
        pdb.addFeature(id, name.toStdString(), face.camera_fea);

        model->setData(model->index(index.row(), 5), true);
        model->submitAll(); // submit will destroy index
    }

}
// revert what you have done
void MainWindow::on_button_revert_clicked()
{
    model->revertAll();
}

// delete facefeature in facefeature.db and delete persons in dbface.db
void MainWindow::on_button_delete_clicked()
{
    int curRow = ui->tableView->currentIndex().row();

    QModelIndex index = ui->tableView->currentIndex();
    index = index.sibling(curRow, 0);

    int id = model->data(index).toInt();
    // id is not equally to row
    pdb.deleteFeature(id);

    model->removeRow(curRow);
    model->submitAll(); // submit will destroy the index
}
