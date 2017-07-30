#ifndef _opticalFlow_H_
#define _opticalFlow_H_

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <eigen3/Eigen/Core>
#include "blockMatch.h"
#include "klt.h"

const int curve_size = 201;

struct Camera
{
	int win_size;
	int width;
	int height;
	float scale;

	cv::Rect roi;
	cv::Mat im_src;

	Camera();
};

class OpticalFlow
{
public:
	float pixel_dis[2];
	Camera *camera;

	OpticalFlow(Camera *_c, std::string _win_name);
	~OpticalFlow();
	
	void sendFrame(const cv::Mat &im, int flg);
	void getOf(int flg);


private:
	std::string win_name;
	int frame_num;
	double fps;
	double time[4];
	Eigen::Matrix3f trans_sum;

	float max_pixel_delta;


	FILE *stream;
	int level_num;
	
	cv::Mat affine;

	cv::Mat im_first;
	cv::Mat im_second;	
	std::vector<cv::Mat> impyr_first;
	std::vector<cv::Mat> impyr_second;

	std::vector<cv::Point2f> corner_first;
    std::vector<cv::Point2f> corner_second;    
	std::vector<uchar> corner_status;
	std::vector<float> corner_err;

	cv::TermCriteria corner_termcrit;
	

	int imX[curve_size];
	int imY[curve_size];


	void findCorner();
	void trackCorner();
	void computeAffine();
	void show();
	void message();
	void getUniformCorner(int num, std::vector<cv::Point2f> &corner);
	void debugDrawCurve(float x, float y);
	
	// 位移投票
	void meanshift();

	// 块匹配
	Blockof blockof;
	void blockOpticalFlow();
	void blockMatch(const cv::Point2f &p, cv::Point2f &p2, uchar &flg);

	// stan光流
	KLT_TrackingContext tc;
	void stanKlt();
};

#endif