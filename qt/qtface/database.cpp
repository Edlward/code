#include "database.h"
#include "mainwindow.h"
#include <cstring>
#include <QDebug>

Database::Database()
{
    if(sqlite3_open((db_path + "facefeature.db").c_str(), &db_feature))
    {
       qDebug() << "can not open facefeature.db";
    }

    if(sqlite3_open((db_path + "dbface.db").c_str(), &db_person))
    {
        qDebug() << "can not open dbface.db";
    }
}

Database::~Database()
{
    if(db_feature != NULL)
    {
        sqlite3_close(db_feature);
    }

    if(db_person != NULL)
    {
        sqlite3_close(db_person);
    }
}

bool Database::getFeature(int &index, float *fea)
{
    char *sql;
    std::sprintf(sql, "select feature from facefeature where id = %d", index);
    sqlite3_prepare(db_feature, sql, std::strlen(sql), &stmt, NULL);
    int rc = sqlite3_step(stmt);

    qDebug() << rc;
    qDebug() << sqlite3_errmsg(db_feature);

}

