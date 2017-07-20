#include <eigen3/Eigen/Eigen>
#include "tool.h"

using namespace cv;
using namespace std;

/****************************************************************
 * 使用eigen库计算放射变换
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


/****************************************************************
 * 图像预处理
****************************************************************/
void btPreprocess(Mat &src)
{
    Mat isc(src.cols, src.rows, CV_8UC1);

    int max = 0xff;
    int min = 0;
    int x_step = 1;
    int y_step = 3;

    for(int i = 0; i < src.rows-y_step; ++i)
    {
        uchar *p_src1 = src.ptr(i);
        uchar *p_src2 = src.ptr(i+y_step) + x_step;
        
        uchar *p_dst = isc.ptr(i);
        for(int j = 0; j < src.cols - x_step; ++j)
        {
            if(*p_src1 < *p_src2)
            {
                *p_dst = min;
            }
            else
            {
                *p_dst = max;
            }
			p_src1++;
			p_src2++;
			p_dst++;
        }
    }
    swap(src, isc);
	// isc.copyTo(src);
}

/****************************************************************
 * 直方图做统计，计算适合的全局运动量
****************************************************************/
void histAffine(vector<Point2f> &p_src, vector<Point2f>  &p_dst, vector<uchar> &status)
{

}
/*************************************************************
 * 
**************************************************************/
void lowPassFilter(float *src, float *dst)
{
    static float src_last[3][2] = {0};
    float a0 = 0.8, a1 = 0.1, a2 = 0.1;
    float src_media[2];

    src_media[0] = src[0];
    src_media[1] = src[1];

#define MEDIA_FILTER

#ifdef MEDIA_FILTER
    // x中值滤波
    if(src[0] < src_last[0][0])
    {
        if(src[0] < src_last[1][0])
        {
            if(src_last[0][0] < src_last[1][0])
            {
                src_media[0] = src_last[0][0];
            }
            else
            {
                src_media[0] = src_last[1][0];
            }
        }
    }
    else
    {
        if(src[0] > src_last[1][0])
        {
            if(src_last[0][0] < src_last[1][0])
            {
                src_media[0] = src_last[1][0];
            }
            else
            {
                src_media[0] = src_last[0][0];
            }
        }
    }

    // y中值滤波
    if(src[1] < src_last[0][1])
    {
        if(src[1] < src_last[1][1])
        {
            if(src_last[0][1] < src_last[1][1])
            {
                src_media[1] = src_last[0][1];
            }
            else
            {
                src_media[1] = src_last[1][1];
            }
        }
    }
    else
    {
        if(src[1] > src_last[1][1])
        {
            if(src_last[0][1] < src_last[1][1])
            {
                src_media[1] = src_last[1][1];
            }
            else
            {
                src_media[1] = src_last[0][1];
            }
        }
    }
#endif


    // 因为src 和dst可能是同一个地址，所以要先保存原来的数据
    src_last[2][0] = src_last[1][0];
    src_last[1][0] = src_last[0][0];
    src_last[0][0] = src[0];

    src_last[2][1] = src_last[1][1];
    src_last[1][1] = src_last[0][1];
    src_last[0][1] = src[1];

    
    // 低通滤波
    dst[0] = src_media[0] * a0 + src_last[1][0] * a1 + src_last[2][0] * a2;
    dst[1] = src_media[1] * a0 + src_last[1][1] * a1 + src_last[2][1] * a2;
}