#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
#include <vector>
#include "xtofglobal.h"
#include "xtofFindCorner.h"
#include "xtofCalcPyrlk.h"
#include "xtofAffine2D.h"
#include "xtofCameraHeight.h"
#include "xtofCameraCorrect.h"
#include <opencv2/opencv.hpp>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <time.h>
#include "opticalFlow.h"

extern "C"{
#include "readSonar.h"
#include "PilotData.h"
};

using namespace cv;

TermCriteria cornerTermcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);//迭代次数和迭代精度
Mat imgPrev;                    //前一帧图像，最新从视频流获取到的在前面
Mat imgPost;                    //后一帧图像
Mat affine2D(2,3,CV_64F);
vector<Mat> imgPyrPrev;
vector<Mat> imgPyrPost;
int maxLevelNum = 1;

float g_cameraHeight = 100.0f; //单位cm
float g_cameraShiftX = 0.0f;
float g_cameraShiftY = 0.0f;

float cameraShiftSumX = 0.0f, cameraShiftSumY = 0.0f;
double sumTime;



int main(int argc, char **argv)
{

	Camera *camera = new Camera;

	OpticalFlow of(camera, "gray");

	OpticalFlow of2(camera, "binary");
	/* init camera */
	int cap_index = 0;
	if(argc > 1)
	{
		sscanf(argv[1], "%d", &cap_index);
	}

	VideoCapture cap;
	cap.open(cap_index);

	cap.set(CV_CAP_PROP_FPS, 30);
	
	// cap.open("/home/lxg/codedata/live.avi");
	if(!cap.isOpened())
	{
		fprintf(stderr, "Can't initialize cam!\n");
		return -1;
	}
    else
    {
        fprintf(stderr, "open camera successd\n");
    }
	// cap.get()
	camera->width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	camera->height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	if(camera->width < camera->roi.width || 
		camera->height < camera->roi.height)
	{
		printf("camera roi is larger than camera\n");
		return -1;
	}
	camera->roi.x = (camera->width - camera->roi.width) / 2;
	camera->roi.y = (camera->height - camera->roi.height) / 2;

	
	Mat frame;

    while (1)
	{
		
		cap >> frame;
		if (frame.empty())
		{
			fprintf(stderr,"video3ss.avi end\n");
			break;
		}

		of.sendFrame(frame, 0); //是否做Bt预处理
		of.getOf(1); //是用LK或者块匹配

		// of2.sendFrame(frame, 1); //binary
		// of2.sendFrame(frame, 1);
		// of2.getOf(1);

		char c = waitKey(1);
		if(c == 27)
		{
			break;
		}
		// image = frame(camera.roi);
		// //remap(image, image, cameraMapX, cameraMapY, INTER_LINEAR);//图像摄像机畸变矫正
		// resize(image, image, Size(), camera.scale, camera.scale);
		// cvtColor(image, imgPrev, COLOR_BGR2GRAY);
		
		// xtofOpticalFlow();
	}
	cap.release();
	delete camera;
	return 0;
}

int xtofOpticalFlow()
{
	Camera camera;

	vector<Point2f> cornerPrev, cornerPost;
	vector<uchar> cornerStatus;//
	vector<float> cornerErr; //
	vector<Point2f> cornerPrevC,cornerPostC;
	int cornerNum;
	Mat affineInlier;

	affine2D.setTo(0);
#ifdef DEBUG
	static int imgNum = 0;//图像序号
#endif
	double funTime = (double)cvGetTickCount();

	//寻找易跟踪的角点
	xtofCornerToTrack(imgPost, cornerPost, Mat(), 3);

	if(cornerPost.size() > 6)
	{
		//光流跟踪
		maxLevelNum = buildOpticalFlowPyramid(imgPrev, imgPyrPrev, Size(WIN_SIZE,WIN_SIZE), maxLevelNum, false);
		//0.002这个参数非常重要，太大了会判断为跟踪失败	
		xtofCalcPyrlk(imgPyrPost, imgPyrPrev, cornerPost, cornerPrev, cornerStatus, cornerErr, cornerTermcrit, 0, 0.002L, maxLevelNum);
		
		//成功跟踪角点提取
		for (cornerNum = 0; cornerNum < cornerStatus.size(); cornerNum++)
		{
			if (cornerStatus[cornerNum])
			{
				cornerPrevC.push_back(cornerPrev[cornerNum]);
				cornerPostC.push_back(cornerPost[cornerNum]);
			}
		}

		
		affineInlier.create(1,cornerPrevC.size(),CV_8U);
		affineInlier.setTo(1);
		//仿射变换矩阵求取	

		if(cornerPrevC.size() > 5)
		{
			xtofAffine2D(cornerPrevC, cornerPostC, affine2D, affineInlier, 2, 0.96);
		}
	}
	float sonar = 0;
	// sonar = readSonar(fd);
	//更新高度，激光高度获取	
	// xtofCameraHeight(g_cameraHeight);


	//计算实际位移
	//方案一
	g_cameraShiftX = (float)(affine2D.at<double>(0, 2) / CAMERA_FOCAL_X * g_cameraHeight / camera.scale);
	g_cameraShiftY = (float)(affine2D.at<double>(1, 2) / CAMERA_FOCAL_Y * g_cameraHeight / camera.scale);

	funTime = ((double)cvGetTickCount() - funTime) / (cvGetTickFrequency() * 1000);
	// fprintf(stderr,"funtime:%f    %f\t%f\t%f\n",funTime, sonar,g_cameraShiftX,g_cameraShiftY);
	//distanceSumX += distanceX;
	//distanceSumY += distanceY;
	//方案二
	//利用视场角、高度信息，待写
	//位移滤波

	//按电子罗盘方向分解x,y方向位移并求和，视频调试
	//static int i = 0;
	//++i;
	swap(imgPyrPost, imgPyrPrev);
	swap(imgPost, imgPrev);
	

#ifdef DEBUG
	++ imgNum;//图像序号
	cout << imgNum << "\t" << cornerNum << '\t';	
	cout.width(7);
	cout.precision(4);
	cout.setf(ios::fixed);//小数点后精确几位
	//cout << tSum / (cvGetTickFrequency() * 1000 << endl;
	cout << affine2D.at<double>(0, 0) << '\t' << affine2D.at<double>(0, 1) << '\t' 
		<< affine2D.at<double>(1, 0) << "\t" << affine2D.at<double>(1, 1) << "\t\t" 
        << "dx:" <<g_cameraShiftX << '\t' << "dy" << g_cameraShiftY << "\t";

	cameraShiftSumX += g_cameraShiftX;
	cameraShiftSumY += g_cameraShiftY;
	

	//先把数据复制出来,以免画上面的点影响后续检测
	Mat imgPostTmp;
	imgPost.copyTo(imgPostTmp);
	for (int i = 0; i < cornerPostC.size(); i++)
	{
		if (1 == affineInlier.at<unsigned char>(i))
		{
			circle(imgPrev, cornerPostC[i], 1, Scalar(255, 0, 0), -1, 8);//因为swap函数imgpPrev实为post
			circle(imgPostTmp, cornerPrevC[i], 1, Scalar(255, 0, 0), -1, 8);//因为swap函数imgPost实为Prev
		}
		else
		{
			circle(imgPrev, cornerPostC[i], 2, Scalar(255, 0, 0), -1, 8);//因为swap函数imgpPrev实为post
			circle(imgPostTmp, cornerPrevC[i], 2, Scalar(255, 0, 0), -1, 8);//因为swap函数imgPost实为Prev
		}
	}
	//调试时保存图片
	//char str1[30],str2[30];
	//sprintf_s(str1, "img\\img_%d_Post.bmp", imgNum);
	//cv::imwrite(str1, imgPrev);
	//sprintf_s(str2, "img\\img_%d_Prev.bmp", imgNum);
	//cv::imwrite(str2, imgPostTmp);
    cout << endl;
	imshow("camera",imgPrev);
	char key = waitKey(1);
    if(key == 27)
    {
        exit(0);
    }
#endif

	return 1;
}
