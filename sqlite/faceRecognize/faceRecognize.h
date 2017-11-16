#ifndef __FACERECOGNIZE_H_
#define __FACERECOGNIZE_H_

#include <string>
#include <sqlite3.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class FaceRecognize
{
public:
    FaceRecognize(std::string dbname)
    {
        db = nullptr;
        if(sqlite3_open(dbname.c_str(), &db))
        {
            std::cout << "can not open sql database" << std::endl;
        }
    }

    ~FaceRecognize()
    {
        if(db != nullptr)
        {
            sqlite3_close(db);
        }
    }

    
    virtual bool searchDb(cv::Mat &im)
    {
        std::cout << "abstract recognize clas has been called" << std::endl;
    }
    
    sqlite3 *db;
    sqlite3_stmt *stmt; 

    std::vector<std::string> name;
private:    

};

#endif