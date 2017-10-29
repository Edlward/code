#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

class Database
{
public:
    Database();
    ~Database();

    sqlite3 *db_feature;
    sqlite3 *db_person;
    sqlite3_stmt *stmt;

    bool getFeature(int &index, float *fea);
private:

};
#endif // DATABASE_H
