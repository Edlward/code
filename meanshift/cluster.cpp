#include "cluster.h"
#include <stdio.h>

using namespace cv;
using namespace std;

void Cluster::loadData(int num)
{
    RNG rng;
    data.resize(100);
    for(int i = 0; i < num/3; ++i)
    {
        data[i].x = rng.gaussian(1);
        data[i].y = rng.gaussian(1);
    }
    for(int i = num/3; i < num*2/3; ++i)
    {
        data[i].x = rng.gaussian(1) + -10;
        data[i].y = rng.gaussian(1) + -8;
    }
    for(size_t i = num*2/3; i < data.size(); ++i)
    {
        data[i].x = rng.gaussian(1) + 5;
        data[i].y = rng.gaussian(1) + 8;
    }

    for(const auto &iter:data)
    {
        printf("%f,%f\n", iter.x, iter.y);
    }
}
void Cluster::meanshift()
{
    vector<Point2f> center;
    vector<bool> center_flg;
    center.resize(data.size());
    center_flg.resize(data.size());
    for(size_t i = 0; i < center.size(); ++i)
    {
        center_flg[i] = false;
        center[i] = data[i];
    }

    float class_distance = 5.f; //把小于这个距离的点当作一个类别
    float min_distance = 0.002;  //每次偏移的最小距离
    float max_min_distance = min_distance + 1; //保留每轮更新的最大偏移
    int counter = 0;
    float dist = 0.f;

    while(max_min_distance > min_distance)
    {
        ++counter;
        float max_shift = 0;
        printf("iterate count:%d\n", counter);
        for(size_t i = 0; i < center_flg.size(); ++i)
        {
            if(center_flg[i])    
            {
                continue;
            } 
            Point2f shift_sum(0.f,0.f);
            int shift_count = 0;
            for(size_t j = 0; j < center_flg.size(); ++j)
            {
                // opencv 计算二范数
                dist = norm(center[i] - data[j]);
                if(dist < class_distance)
                {
                    ++shift_count;
                    shift_sum = shift_sum + center[i] - data[j];
                }
            }
            if(shift_count)
            {
                shift_sum = shift_sum * (1.0f / shift_count);
            }

            float shift_sum_dist = norm(shift_sum);
            if(shift_sum_dist > max_shift)
            {
                max_shift = shift_sum_dist;
            }
            if(shift_sum_dist < min_distance)
            {
                center_flg[i] = true;
                continue;
            }
            center[i] -= shift_sum;
        }

        max_min_distance = max_shift;
        printf("max_min_distance:%f\n", max_min_distance);
    }
    // 根据center归类
    for(const auto &iter:center_flg)
    {
        cout << iter << "\t";
    }
    printf("\n");

    for(const auto &iter:center)
    {
        printf("%f,%f\t\n", iter.x, iter.y);
    }
    printf("\n");
    
}