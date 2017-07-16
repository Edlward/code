#include "opticalFlow.h"

#include "xtofFindCorner.h"
#include "xtofCalcPyrlk.h"
#include "xtofAffine2D.h"
#include "xtofCameraHeight.h"
#include "xtofCameraCorrect.h"

#include "tool.h"

using namespace std;
using namespace cv;
/****************************************************************
 * 设置camera结构体
****************************************************************/
OpticalFlow::OpticalFlow():level_num(1),
                affine(2, 3, CV_64FC1, Scalar(0)),
                corner_termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03)//迭代次数和迭代精度
{
    stream = stdout;
    // stream = fopen("/home/lxg/codedata/ofdata.txt", "w");

    camera = new Camera;

    camera->scale = 1.f;
    camera->roi.width = 200;
    camera->roi.height = 200;
    camera->win_size = 21;

    max_pixel_delta = 2.5f; //函数没有放在类内，所以此处更改不起作用
    

    im_first = Mat::zeros(camera->scale*camera->roi.width, 
                        camera->scale*camera->roi.height, CV_8UC1);
    level_num = buildOpticalFlowPyramid(im_first, impyr_first, Size(camera->win_size, 
                        camera->win_size), level_num, false);           
    fprintf(stream, "max_level:%d\n", level_num);
}
/****************************************************************
 * 读取图像
****************************************************************/
void OpticalFlow::sendFrame(const Mat &im)
{
    cv::swap(im_first, im_second);
    swap(impyr_first, impyr_second);

    Mat tmp;
    cvtColor(im(camera->roi), tmp, CV_RGB2GRAY);
    resize(tmp, im_first, Size(), camera->scale, camera->scale); 
    buildOpticalFlowPyramid(im_first, impyr_first, Size(camera->win_size, 
                        camera->win_size), level_num, false); 
}
/****************************************************************
 * 
****************************************************************/
void OpticalFlow::getOf()
{
    double t = (double)getTickCount();
    
    // findCorner();
    // trackCorner();
    blockOpticalFlow();
    computeAffine();

    time[3] = ((double)getTickCount() - t) / getTickFrequency() * 1000;
    
    show();
    message();
}
/****************************************************************
 * 
****************************************************************/
void OpticalFlow::findCorner()
{
    double t = (double)getTickCount();
    // getUniformCorner(10, corner_second);
    xtofCornerToTrack(im_second, corner_second, Mat(), 3);
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
    
    vector<Point2f> corner_firstc;
    vector<Point2f> corner_secondc;

    corner_firstc.clear();
    corner_secondc.clear();
	//成功跟踪角点提取
    if(corner_first.empty())
    {
        return;
    }
    findFundamentalMat(corner_first, corner_second, FM_RANSAC, 2, 0.99, corner_status);
	for (int i = 0; i < corner_status.size(); i++)
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
        eigenAffine(corner_firstc, corner_secondc, pixel_dis);

		// xtofAffine2D(corner_firstc, corner_secondc, affine, affine_inliner, 2, 0.96);
        // pixel_dis[0] = affine.at<double>(0,2);
        // pixel_dis[1] = affine.at<double>(1,2);
        
        // cout << "ransac\n" <<  affine << endl;
	}
    time[2] = ((double)getTickCount() - t) / getTickFrequency()*1000;
}
/****************************************************************
 * 显示角点，及跟踪效果
****************************************************************/
void OpticalFlow::show()
{
    Mat tmp;
    im_second.copyTo(tmp);
    cvtColor(tmp, tmp, CV_GRAY2RGB);

    Scalar color(0,255,0);
    Scalar color_err(0,0,255);
    for(int i=0; i < corner_second.size(); ++i)
    {
        if(corner_status[i])
        {
            circle(tmp, corner_second[i], 1, color, 1);
            line(tmp, corner_first[i], corner_second[i], color);
        }
        else
        {
            circle(tmp, corner_second[i], 1, color_err, 1);
            line(tmp, corner_first[i], corner_second[i], color_err);
        }
    }
    Point2f p_center, p_center_of;
    p_center.x = tmp.cols/2;
    p_center.y = tmp.rows/2;
    p_center_of.x = p_center.x + pixel_dis[0]*5;
    p_center_of.y = p_center.x + pixel_dis[1]*5;
    line(tmp, p_center, p_center_of, color_err);

    debugDrawCurve(pixel_dis[0], pixel_dis[1]);

    resize(tmp, tmp, Size(400,400));

    static int save_num = 0;
    string path = "/home/lxg/codedata/opticalFlow/";
    imwrite((path + to_string(save_num) + ".jpg").c_str(), tmp);
    save_num++;
    
    imshow("imSecond", tmp);
}
/****************************************************************
 * 
****************************************************************/
void OpticalFlow::message()
{
    // fprintf(stream, "level_num:%d \t", (int)impyr_second.size());
    fprintf(stream, "cornerNum:%d\t", (int)corner_second.size());
    fprintf(stream, "time:%-3.2f %-3.2f %3.2f   %3.2f \t", time[0], time[1], time[2], time[3]);
    // fprintf(stream, "fps:%3.f\t", fps);
    fprintf(stream, "dx: %1.4f,%1.4f", std::abs(pixel_dis[0]), std::abs(pixel_dis[1]));
    fprintf(stream, "\n");
}
/****************************************************************
 * 
****************************************************************/
OpticalFlow::~OpticalFlow()
{
    delete camera;
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

