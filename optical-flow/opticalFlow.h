#ifndef _opticalFlow_H_
#define _opticalFlow_H_

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <eigen3/Eigen/Core>

#include "blockMatch.h"

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
	Camera *camera;

	OpticalFlow(Camera *_c, std::string _win_name);
	~OpticalFlow();
	
	void sendFrame(const cv::Mat &im, int flg);
	void getOf(int flg);


private:
	std::string win_name;

	double fps;
	double time[4];
	float pixel_dis[2];
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

	cv::TermCriteria corner_termcrit;
	std::vector<uchar> corner_status;
	std::vector<float> corner_err;
	
	void findCorner();
	void trackCorner();
	void computeAffine();
	void show();
	void message();
	void getUniformCorner(int num, std::vector<cv::Point2f> &corner);

	Blockof blockof;
	void blockOpticalFlow();
	void blockMatch(const cv::Point2f &p, cv::Point2f &p2, uchar &flg);
};

#endif