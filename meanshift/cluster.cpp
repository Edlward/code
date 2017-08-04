#include "cluster.h"
#include <stdio.h>

using namespace cv;
using namespace std;

void Cluster::loadData(int num)
{
    RNG rng;
    data.resize(num*4/3);
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
    for(int i = num*2/3; i < num; ++i)
    {
        data[i].x = rng.gaussian(1) + 5;
        data[i].y = rng.gaussian(1) + 8;
    }

    for(int i = num; i < num*4/3; ++i)
    {
        data[i].x = rng.gaussian(1) + -10;
        data[i].y = rng.gaussian(1) + -8;
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

    // 根据center归类,统计类别数，归类
    vector<Point2i> sc; //shift class
    vector<int> sc_num;
    vector<int> sc_flg(center.size()); //类别标签
    Point2i ptmp;
    for(size_t i = 0; i < center.size(); ++i)
    {
        ptmp.x = floor(center[i].x * 1000);
        ptmp.y = floor(center[i].y * 1000);

        size_t j = 0;
        bool flg = false;
        for(; j < sc.size(); ++j)
        {
            if(ptmp == sc[j])
            {
                ++sc_num[j];
                flg = true;
                sc_flg[i] = j;
                break;
            }
        }
        if(!flg)
        {
            sc_flg[i] = (int)sc.size();
            sc_num.push_back(1);
            sc.push_back(ptmp);
        }
    }
    printf("sc_num:%d\n", (int)sc_num.size());
    for(size_t i = 0; i < sc_num.size(); ++i)
    {
        printf("order:%d num:%d x:%d,y:%d\n", (int)i, (int)sc_num[i], 
                sc[i].x, sc[i].y);
    }   
    printf("class flag:\n");
    for(const auto &iter:sc_flg)
    {
        printf("%d\t", iter);
    } 
    printf("\n");
    //冒泡排序求类别数目最大值
    int max_num = 0;
    int max_index;
    for(size_t i = 0; i < sc_num.size(); ++i)
    {
        if(max_num < sc_num[i])
        {
            max_index = i;
            max_num = sc_num[i]; 
        }
    }
    printf("index:%d, num:%d\n", max_index, max_num);

}