#ifndef _opticalFlow_H_
#define _opticalFlow_H_

#include <opencv2/opencv.hpp>
#include <vector>
#include "blockMatch.h"

struct Camera
{
	int win_size;
	int width;
	int height;
	float scale;

	cv::Rect roi;
};

class OpticalFlow
{
public:
	Camera *camera;

	OpticalFlow();
	~OpticalFlow();
	
	void sendFrame(const cv::Mat &im);
	void getOf();


private:
	double fps;
	double time[4];
	float pixel_dis[2];
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
	void blockMatch(const cv::Point2f &p, cv::Point2f &p2);
};

#endif