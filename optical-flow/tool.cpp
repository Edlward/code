#include <eigen3/Eigen/Eigen>
#include "tool.h"

using namespace cv;
using namespace std;

const int curve_size = 201;

void debugDrawCurve(float x, float y)
{
	Size winSize(700, 400);
	Mat im = Mat::zeros(winSize, CV_8UC3);
	
	
	static int imX[curve_size];
	static int imY[curve_size];

	int arrayLength = curve_size - 1;
	int max = 150, min = -150;

	x *= 10;
	y *= 10;
	if (x > max) x = max;
	if (x < min) x = min;
	if (y > max) y = max;
	if (y < min) y = min;

	imX[arrayLength] = (int)(x + winSize.height / 2);
	imY[arrayLength] = (int)(y + winSize.height / 2);

	for (int i = 0; i < arrayLength; ++i)
	{
		imX[i] = imX[i + 1];
		imY[i] = imY[i + 1];
	}
	
	line(im, Point2d(0, winSize.height / 2), Point2d(winSize.width, winSize.height / 2), Scalar(255,255,255));
	line(im, Point2d(0, winSize.height / 2 + max), Point2d(winSize.width, winSize.height / 2 + max), Scalar(255, 255, 255));
	line(im, Point2d(0, winSize.height / 2 + min), Point2d(winSize.width, winSize.height / 2 + min), Scalar(255, 255, 255));

	for (int i = arrayLength; i > 0; --i)
	{
		line(im, Point2d(i * 3, imX[i]), Point2d((i - 1) * 3, imX[i - 1]), Scalar(0, 0, 255), 1);
		line(im, Point2d(i * 3, imY[i]), Point2d((i - 1) * 3, imY[i - 1]), Scalar(0, 255, 0), 1);
		circle(im, Point2d(i * 3, imX[i]), 2, Scalar(0, 0, 255), -1, 8);
		circle(im, Point2d(i * 3, imY[i]), 2, Scalar(0, 255, 0), -1, 8);
	}
	imshow("curve", im);
}
/****************************************************************
 * 
****************************************************************/
void eigenAffine(const vector<Point2f> &src, const vector<Point2f> &dst, float *dis)
{
    Eigen::MatrixXf e_src(2, src.size());
    Eigen::MatrixXf e_dst(2, dst.size());

    for(int i=0; i < src.size(); ++i)
    {
        e_src(0, i) = src[i].x;
        e_src(1, i) = src[i].y;
    }
    
    for(int i=0; i < dst.size(); ++i)
    {
        e_dst(0, i) = dst[i].x;
        e_dst(1, i) = dst[i].y;
    }
    
    // 对于2D点的仿射变换矩阵是3*3矩阵
    Eigen::Matrix3f trans;
    trans = Eigen::umeyama(e_src, e_dst, false);
    dis[0] = trans(0,2);
    dis[1] = trans(1,2);
    // cout << "eigen\n" << trans << endl;
}