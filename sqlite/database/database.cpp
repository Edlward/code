#include "database.h"
#include <iostream>
#include <cstring>
#include <typeinfo>

SqlDatabase::SqlDatabase(std::string dbname)
{
    db = nullptr;
    if(sqlite3_open(dbname.c_str(), &db))
    {
        std::cout << "can not open sql database" << std::endl;
    }
}

SqlDatabase::~SqlDatabase()
{
    if(db != nullptr)
    {
        sqlite3_close(db);
    }
}

bool SqlDatabase::getRecord(int index)
{
    std::string sql = "select * from persons";
    int rc = sqlite3_prepare(db, sql.c_str(), sql.length(), &stmt, NULL);
    if(rc != SQLITE_OK)
    {
        std::cout << sqlite3_errmsg(db);
        return false;
    }

    while(1)
    {
        rc = sqlite3_step(stmt);
        if(rc == SQLITE_DONE)
        {
            break;
        }
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *person_name = sqlite3_column_text(stmt, 1);
        const unsigned char *image_name = sqlite3_column_text(stmt, 2);
        
        // ansci表，可查询数值是对的，这里是当做字符输出的，所以显示有问题
        const unsigned char *feature = reinterpret_cast<const unsigned char *>(sqlite3_column_blob(stmt, 3));
        std::cout << "id: " << id << "\tperson_name: " << person_name << "\timage_name: " 
                    << image_name << "\tfeature0: " << typeid(feature).name() << "\t" << feature[1] << "\t" << feature[1]+1 << std::endl;
    }

    return true;
}