#include "backstraction.h"

using namespace cv;

Backstraction::Backstraction()
{

}

void Backstraction::prepareBackFrame(const Mat &src)
{
    src.copyTo(f2);
}

void Backstraction::subtraction(const Mat &f1, Mat &result)
{
    if(f1.empty())
    {
        prepareBackFrame(f1);
    }

    Mat tmp;
    absdiff(f2, f1, tmp);
    threshold(tmp, result, 15, 255, THRESH_BINARY);

    prepareBackFrame(f1);
}