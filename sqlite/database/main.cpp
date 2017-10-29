#include <sqlite3.h>
#include "database.h"
#include <iostream>

int main()
{
    SqlDatabase database("/home/lxg/Qt5.8.0/mycode/build-test-Desktop_Qt_5_8_0_GCC_64bit-Debug/faceData.db");
    database.getRecord(1);
    unsigned char c = 'c';
    printf("printf: %d\n", c);
    std::cout << "cout c:" << c << std::endl;
}