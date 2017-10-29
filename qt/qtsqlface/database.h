#ifndef INIDB_H
#define INIDB_H

// it can include qtsql only set Qt += sql in .pro
#include <QtSql>
#include <sqlite3.h>

class PersonDatabase
{
 public:
    sqlite3 *db;
    sqlite3_stmt *stmt;

    PersonDatabase();
    ~PersonDatabase();

    void addFeature(const int &id, const std::string &name, const float *feature);
    void deleteFeature(const int &id);

    QSqlError getConnection();

};

#endif // INIDB_H
