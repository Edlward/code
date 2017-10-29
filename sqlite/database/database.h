#ifndef __DATABASE_H_
#define __DATABASE_H_

#include <sqlite3.h>
#include <string>

class SqlDatabase
{
public:
    SqlDatabase(std::string dbname);
    ~SqlDatabase();

    bool getRecord(int index);

private:    
    sqlite3 *db;
    sqlite3_stmt *stmt;
};

#endif