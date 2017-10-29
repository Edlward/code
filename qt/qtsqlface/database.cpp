#include "database.h"
#include "mainwindow.h"
#include <cstring>

QSqlError PersonDatabase::getConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    std::string db_file = db_path + "dbface.db";
    db.setDatabaseName(db_file.c_str());

    if(!db.open())
    {
        return db.lastError();
    }

    QStringList tables = db.tables();
    if(tables.contains("persons", Qt::CaseInsensitive))
    {
        return QSqlError();
    }

    QSqlQuery q;
    if(!q.exec(QLatin1Literal("create table persons(id integer primary key, image varchar, "
                              "name varchar, age integer, phone varchar, feature integer)")))
    {
        qDebug() << "error in create database";
        return q.lastError();
    }

    return QSqlError();
}

PersonDatabase::PersonDatabase()
{
    if(sqlite3_open((db_path + "facefeature.db").c_str(), &db))
    {
        qDebug() << "can not open facefeature.db";
    }
    std::string sql;
    int rc;

    // verify table
    sql = "select count(*) from sqlite_master where type='table' and name = 'facefeature'";
    rc = sqlite3_prepare(db, sql.c_str(), sql.length(), &stmt, NULL);
    if(rc != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db);
    }
    sqlite3_step(stmt);
    int num = sqlite3_column_int(stmt, 0);

    if(num != 1)
    {
        // create table
        sql = "create table facefeature(id integer primary key, image varchar, feature blob)";

        sqlite3_prepare(db, sql.c_str(), sql.length(), &stmt, NULL);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
}

PersonDatabase::~PersonDatabase()
{
    if(db != NULL)
    {
        sqlite3_close(db);
    }
}

void PersonDatabase::addFeature(const int &id, const std::string &image, const float *feature)
{
    // first delete than insert, just like update
    deleteFeature(id);

    std::string sql = "insert into facefeature(id,image,feature) values(?,?,?);";
    int rc;

    rc = sqlite3_prepare(db, sql.c_str(), sql.length(), &stmt, NULL);
    if(rc != SQLITE_OK)
    {
        qDebug() << "sqlite3" << sqlite3_errmsg(db);
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, image.c_str(), image.length(), NULL);
    sqlite3_bind_blob(stmt, 3, feature, 2048*sizeof(float), NULL);

    qDebug() << "id:" << id;
    qDebug() << "image" << image.c_str();
    qDebug() << "feature" << feature[0];

    sqlite3_step(stmt);
}

void PersonDatabase::deleteFeature(const int &id)
{
    char sql[60];
    std::sprintf(sql, "delete from facefeature where id = %d", id);

    int rc;
    rc = sqlite3_prepare(db, sql, std::strlen(sql), &stmt, NULL);
    if(rc != SQLITE_OK)
    {
        qDebug() << "sqlite3" << sqlite3_errmsg(db);
    }
    sqlite3_step(stmt);
}



