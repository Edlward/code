#include "opticalFlow.h"
#include "blockMatch.h"
#include <cstdio>

using namespace cv;


void OpticalFlow::blockOpticalFlow()
{
    double t = (double)getTickCount();

    // 均匀生成所跟踪的块
    getUniformCorner(5, corner_second);
    
    corner_first.resize(corner_second.size());
    corner_status.resize(corner_second.size());
    
    for(int i = 0; i < corner_second.size(); ++i)
    {
        // fprintf(stream, "blockOf %i\n", i);
        blockMatch(corner_second[i], corner_first[i], corner_status[i]);
    }
    // fprintf(stream, "block of end\n");

    time[1] = ((double)getTickCount() - t) / getTickFrequency() * 1000;
}

void OpticalFlow::blockMatch(const Point2f &p, Point2f &p2, uchar &flg)
{
    Point2i s0, s1;
    s0.x = cvRound(p.x - blockof.search_half_size - blockof.win_half_size);
    s0.y = cvRound(p.y - blockof.search_half_size - blockof.win_half_size);
    s1.x = cvRound(p.x + blockof.search_half_size - blockof.win_half_size);
    s1.y = cvRound(p.y + blockof.search_half_size - blockof.win_half_size);

    Point2i a0, a1;
    a0.x = cvRound(p.x - blockof.win_half_size);
    a0.y = cvRound(p.y - blockof.win_half_size);
    a1.x = cvRound(p.x + blockof.win_half_size);
    a1.y = cvRound(p.y + blockof.win_half_size);

    int cost = 0, cost_least = 0xffffff;
    int px_expect = 0, py_expect = 0;

    for(int i = s0.y; i < s1.y; ++i)
    {
        for(int j = s0.x; j < s1.x; ++j)
        {
            cost = 0;
            // 小窗口循环
            for(int m = 0; m < 2*blockof.win_half_size; ++m)
            {
                uchar *p_first = im_first.ptr(i + m); // 确定行首地址
                uchar *p_second = im_second.ptr(a0.y + m); // 确定行首地址
                p_first += j;
                p_second += a0.x;

                for(int n = 0; n < 2*blockof.win_half_size; ++n)
                {
                    cost += std::abs(*p_first - *p_second);// * (*p_first - *p_second);
                    // cost += (*p_first - *p_second) * (*p_first - *p_second);
                    
                    p_first++;
                    p_second++;
                }
            }
            // printf("%d,%d cost:%d\n", i, j, cost);

            if(cost < cost_least)
            {
                cost_least = cost;
                px_expect = j;
                py_expect = i;
            }

        }
    }
    // printf("least %d\n", cost_least);
    
    p2.x = px_expect + blockof.win_half_size;
    p2.y = py_expect + blockof.win_half_size;
    
    flg = 1;
}