#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtSql>
#include <QSqlTableModel>

#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    PersonDatabase pdb;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_button_add_clicked();

    void on_button_submit_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_button_revert_clicked();

    void on_button_delete_clicked();

private:
    QSqlTableModel *model;
    Ui::MainWindow *ui;

    void showError(const QSqlError &err)
    {
        QMessageBox::critical(this, "unable to initalize Database",
                              "error initializing database: " + err.text());
    }
};
const QString path_image = "/home/lxg/codedata/face/picture/";
const std::string model_path = "/home/lxg/codedata/face/model/";
const std::string db_path = "/home/lxg/codedata/face/";

#endif // MAINWINDOW_H
