#ifndef _TOOL_H_
#define _TOOL_H_

#include <vector>
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Eigen>


void debugDrawCurve(float x, float y);
void eigenAffine(const std::vector<cv::Point2f> &src, const std::vector<cv::Point2f> &dst, Eigen::Matrix3f &trans);
void btPreprocess(cv::Mat &src);


#endif