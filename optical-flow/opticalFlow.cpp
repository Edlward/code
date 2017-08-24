#include "opticalFlow.h"
#include "xtofFindCorner.h"
#include "xtofCalcPyrlk.h"
#include "xtofAffine2D.h"
#include "xtofCameraHeight.h"
#include "xtofCameraCorrect.h"
#include <string>
#include "tool.h"

using namespace std;
using namespace cv;

/****************************************************************
 * camera结构提初始化
****************************************************************/
Camera::Camera()
{
    scale = 1.f;
    roi.width = 100;
    roi.height = 100;
    win_size = 21;
}

/****************************************************************
 * 
****************************************************************/
OpticalFlow::OpticalFlow(Camera *_c, string _win_name):
                win_name(_win_name),
                level_num(2),
                affine(2, 3, CV_64FC1, Scalar(0)),
                corner_termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03)//迭代次数和迭代精度
{
    stream = stdout;
    // stream = fopen("/home/lxg/codedata/ofdata.txt", "w");
    frame_num = 0;
    camera = _c;

    max_pixel_delta = 2.5f; //函数没有放在类内，所以此处更改不起作用
    
    pixel_dis[0] = 0.f;
    pixel_dis[1] = 0.f;

    im_first = Mat::zeros(camera->scale*camera->roi.width, 
                        camera->scale*camera->roi.height, CV_8UC1);
    level_num = buildOpticalFlowPyramid(im_first, impyr_first, Size(camera->win_size, 
                        camera->win_size), level_num, false);           
    fprintf(stream, "max_level:%d\n", level_num);

    namedWindow(win_name.c_str(), WINDOW_AUTOSIZE);

    trans_sum.setIdentity();

    tc = KLTCreateTrackingContext();
    KLTPrintTrackingContext(tc);
}

/****************************************************************
 * 读取图像
****************************************************************/
void OpticalFlow::sendFrame(const Mat &im, int flg)
{
    ++frame_num;
    cv::swap(im_first, im_second);
    swap(impyr_first, impyr_second);

    // Mat tmp;
    // cvtColor(im(camera->roi), tmp, CV_RGB2GRAY);
    resize(im(camera->roi), im_first, Size(), camera->scale, camera->scale); 
    im_first.copyTo(camera->im_src);
    
    if(flg)
    {
        btPreprocess(im_first);
    }

    buildOpticalFlowPyramid(im_first, impyr_first, Size(camera->win_size, 
                        camera->win_size), level_num, false); 
}

/****************************************************************
 * 
****************************************************************/
void OpticalFlow::getOf(int flg)
{
    double t = (double)getTickCount();
    
    switch(flg)
    {
        case 1:
            // LK光流
            findCorner();
            trackCorner();
            break;
        case 2:
            // 块匹配光流
            blockOpticalFlow();
            break;
        case 3:
            // wiki 上一个开源的lk算法
            stanKlt();
            break;        
        default:
        {
            fprintf(stream, "the method for computing optical flow choose is none\n");
        }
    }

    // 根据聚类排除跟踪点中的异常点
    // meanshift();

    computeAffine();
    lowPassFilter(pixel_dis, pixel_dis);

    // 每计算相邻的9幅图像的结果，输出一次
    if(frame_num == 3)
    {   
        // frame_num = 0;
        pixel_sum[0] = trans_sum(0,2);
        pixel_sum[1] = trans_sum(1,2);
        
        trans_sum.setIdentity();
    }
    
    time[3] = ((double)getTickCount() - t) / getTickFrequency() * 1000;
    
    show();
    message();
}
/****************************************************************
 * 
****************************************************************/
void OpticalFlow::stanKlt()
{
    int nFeatures = 100;
    KLT_FeatureList fl;
    fl = KLTCreateFeatureList(nFeatures);
    
    unsigned char *img1 = im_first.data;
    unsigned char *img2 = im_second.data;
    int nrows = im_first.rows;
    int ncols = im_first.cols;

    KLTSelectGoodFeatures(tc, img1, ncols, nrows, fl); 
    corner_first.resize(nFeatures);
    for(int i = 0; i < nFeatures; ++i)
    {
        corner_first[i].x = fl->feature[i]->x;
        corner_first[i].y = fl->feature[i]->y;
    }   

    // getUniformCorner(10, corner_first);
    // for(int i = 0; i < corner_first.size(); ++i)
    // {
    //     fl->feature[i]->x = corner_first[i].x;
    //     fl->feature[i]->y = corner_first[i].y;
    // }

    KLTTrackFeatures(tc, img1, img2, ncols, nrows, fl);
    corner_second.resize(nFeatures);
    corner_status.resize(nFeatures);
    for(int i = 0; i < nFeatures; ++i)
    {
        corner_second[i].x = fl->feature[i]->x;
        corner_second[i].y = fl->feature[i]->y;
        if(0 == fl->feature[i]->val)
        {
            corner_status[i] = 1;
        }
        else
        {
            corner_status[i] = 0;
        }
        // printf("flg:%d cornerFirst x:%f, y:%f, second x:%f, y:%f \n", 
        //         corner_status[i], corner_first[i].x, corner_first[i].y, 
        //         corner_second[i].x, corner_second[i].y);
    }
}
/****************************************************************
 * 
****************************************************************/
void OpticalFlow::findCorner()
{
    double t = (double)getTickCount();
    getUniformCorner(10, corner_second);
    // xtofCornerToTrack(im_second, corner_second, Mat(), 3);

    time[0] = ((double)getTickCount() - t) / getTickFrequency()*1000;
}
/****************************************************************
 * 
****************************************************************/
void OpticalFlow::trackCorner()
{
    double t = (double)getTickCount();
    
    xtofCalcPyrlk(impyr_second, impyr_first, corner_second, corner_first, 
                    corner_status, corner_err, corner_termcrit, 
                    CV_LKFLOW_GET_MIN_EIGENVALS,// 或者0
                    0.0002L, level_num);
    time[1] = ((double)getTickCount() - t) / getTickFrequency()*1000;
                    
}
/****************************************************************
 * 
****************************************************************/
void OpticalFlow::computeAffine()
{
    double t = (double)getTickCount();
    
    corner_firstc.clear();
    corner_secondc.clear();
	//成功跟踪角点提取
    if(corner_first.empty())
    {
        return;
    }

    // 可屏蔽有的光流算法已经设置了错误跟踪的状态
    // findFundamentalMat(corner_first, corner_second, FM_RANSAC, 2, 0.99, corner_status);
	for (size_t i = 0; i < corner_status.size(); i++)
	{
		if (corner_status[i])
		{
			corner_firstc.push_back(corner_first[i]);
			corner_secondc.push_back(corner_second[i]);
		}
	}

    Mat affine_inliner;
	affine_inliner.create(1,corner_firstc.size(),CV_8U);
	affine_inliner.setTo(1);

	//仿射变换矩阵求取	
	if(corner_firstc.size() > 5)
	{
        // Mat tmp = findFundamentalMat(corner_firstc, corner_secondc);
        // cout << endl << tmp << endl;
        Eigen::Matrix3f trans_tmp;
        eigenAffine(corner_firstc, corner_secondc, trans_tmp);

        pixel_dis[0] = trans_tmp(0,2);
        pixel_dis[1] = trans_tmp(1,2);
        trans_sum = trans_tmp * trans_sum;
        
		// xtofAffine2D(corner_firstc, corner_secondc, affine, affine_inliner, 2, 0.96);
        // pixel_dis[0] = affine.at<double>(0,2);
        // pixel_dis[1] = affine.at<double>(1,2);
        
        // cout << "ransac\n" <<  affine << endl;
	}

    fprintf(stream, "corner correct %s, %d\n", win_name.c_str(), (int)corner_firstc.size());

    time[2] = ((double)getTickCount() - t) / getTickFrequency()*1000;
}

/****************************************************************
 * 显示角点，及跟踪效果
****************************************************************/
void OpticalFlow::show()
{
    Mat tmp;
    camera->im_src.copyTo(tmp);
    cvtColor(tmp, tmp, CV_GRAY2RGB);

    Scalar color(0,255,0);
    Scalar color_err(0,0,255);
    
    
    for(size_t i = 0; i < corner_second.size(); ++i)
    {
        // 正确跟踪
        if(corner_status[i])
        {
            circle(tmp, corner_first[i], 1, color, 1);
            line(tmp, corner_first[i], corner_second[i], color);
        }
        else
        {
            circle(tmp, corner_first[i], 1, color_err, 1);
            // line(tmp, corner_first[i], corner_second[i], color_err);
        }
    }

    if(true)
    {
        Point2f p_center, p_center_of;
        p_center.x = tmp.cols/2;
        p_center.y = tmp.rows/2;
        p_center_of.x = p_center.x - pixel_dis[0]*5;
        p_center_of.y = p_center.x - pixel_dis[1]*5;
        circle(tmp, p_center_of, 1, color_err, 1);
        line(tmp, p_center, p_center_of, color_err);
    }
    
    // 相邻两帧之间的位移
    // debugDrawCurve(pixel_dis[0], pixel_dis[1]); 
    // 相隔多帧图像之间的位移
    // debugDrawCurve(pixel_sum[0], pixel_sum[1]); 
    // 总位移
    debugDrawCurve(trans_sum(0,2), trans_sum(1,2)); 

    resize(tmp, tmp, Size(400,400));

    if(false)
    {
        static int save_num = 0;
        string path = "/home/lxg/codedata/opticalFlow/";
        char str_num[10];
        sprintf(str_num, "%d", save_num);
        imwrite((path + string(str_num) + ".jpg").c_str(), tmp);
        save_num++;
    }
    imshow(win_name.c_str(), tmp);
}
/****************************************************************
 * 
****************************************************************/
void OpticalFlow::message()
{
    fprintf(stream, "%s ", win_name.c_str());
    // fprintf(stream, "level_num:%d \t", (int)impyr_second.size());
    fprintf(stream, "cornerNum:%d\t", (int)corner_second.size());
    fprintf(stream, "time:%-3.2f %-3.2f %3.2f   %3.2f \t", time[0], time[1], time[2], time[3]);
    // fprintf(stream, "fps:%3.f\t", fps);
    fprintf(stream, "dx: %1.4f,%1.4f", std::abs(pixel_dis[0]), std::abs(pixel_dis[1]));
    fprintf(stream, "\n");
    cout << trans_sum << endl;
    
}
/****************************************************************
 * 
****************************************************************/
OpticalFlow::~OpticalFlow()
{
    fclose(stream);
}
/****************************************************************
 * 
****************************************************************/
void OpticalFlow::getUniformCorner(int num, vector<Point2f> &corner)
{
	int width = camera->roi.width * camera->scale;
    int height = camera->roi.height * camera->scale;

    int x_num = num;
    int y_num = x_num;

    float x_step = 1.f * width / (x_num + 1);
    float y_step = 1.f * height / (y_num + 1);

    corner.resize(x_num*y_num);
    for(int i=0; i < y_num; ++i)
    {
        for(int j=0; j < x_num; ++j)
        {
            corner[i*x_num+j] = Point2f(cvRound(x_step*(j+1)), cvRound(y_step*(i+1)));
        }
    }
}


void OpticalFlow::debugDrawCurve(float x, float y)
{
	Size winSize(700, 400);
	Mat im = Mat::zeros(winSize, CV_8UC3);
	
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
	imshow((win_name + "_curve").c_str(), im);
}


void OpticalFlow::meanshift()
{
    vector<Point2f> corner_dist;
    vector<Point2f> corner_dist_tmp;
    vector<bool> center_flg;
    corner_dist.resize(corner_first.size());
    corner_dist_tmp.resize(corner_first.size());
    center_flg.resize(corner_first.size());
    
    for(size_t i = 0; i < corner_dist.size(); ++i)
    {
        center_flg[i] = false;
        corner_dist[i] = corner_first[i] - corner_second[i];
        corner_dist_tmp[i] = corner_first[i] - corner_second[i];
    }

    float class_distance = 3.f; //把小于这个距离的点当作一个类别
    float min_distance = 0.002;  //每次偏移的最小距离
    float max_min_distance = min_distance + 1; //保留每轮更新的最大偏移
    int counter = 0;
    float dist = 0.f;

    // printf("\n\n");
    while(max_min_distance > min_distance)
    {
        ++counter;
        float max_shift = 0.f;
        // printf("iterate count:%d\n", counter);

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
                dist = norm(corner_dist[i] - corner_dist_tmp[j]);
                if(dist < class_distance)
                {
                    ++shift_count;
                    shift_sum = shift_sum + corner_dist[i] - corner_dist_tmp[j];
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
            corner_dist[i] -= shift_sum;
        }

        max_min_distance = max_shift;
        // printf("max_min_distance:%f\n", max_min_distance);

    }

    // for(const auto &iter:center_flg)
    // {
    //     cout << iter << "\t";
    // }
    // printf("\n");

    // for(const auto &iter:corner_dist)
    // {
    //     printf("%f,%f\t\n", iter.x, iter.y);
    // }
    // printf("\n");
    

    // 根据center归类,统计类别数，归类
    vector<Point2i> sc; //shift class
    vector<int> sc_num;
    vector<int> sc_flg(corner_dist.size()); //类别标签
    Point2i ptmp;
    sc.clear();
    for(size_t i = 0; i < corner_dist.size(); ++i)
    {
        ptmp.x = floor(corner_dist[i].x * 1000);
        ptmp.y = floor(corner_dist[i].y * 1000);

        size_t j = 0;
        bool flg = false;
        for(; j < sc.size(); ++j)
        {
            if(ptmp == sc[j])
            {
                sc_flg[i] = j;
                ++sc_num[j];
                flg = true;
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

    // printf("sc_num:%d\n", (int)sc_num.size());
    // for(size_t i = 0; i < sc_num.size(); ++i)
    // {
    //     printf("order:%d num:%d x:%d,y:%d\n", (int)i, (int)sc_num[i], 
    //             sc[i].x, sc[i].y);
    // }   
    // printf("class flag:\n");
    // for(const auto &iter:sc_flg)
    // {
    //     printf("%d\t", iter);
    // } 
    // printf("\n");
    

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
    // printf("index:%d, num:%d\n", max_index, max_num);


    // 选取成员数目最多的类为实际位移
    for(size_t i = 0; i < corner_status.size(); ++i)
    {
        if(max_index == sc_flg[i])
        {
            corner_status[i] = true;
        }
        else
        {
            corner_status[i] = false;
        }
    }
}