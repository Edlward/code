#ifndef _TOOL_H_
#define _TOOL_H_

#include <vector>
#include <opencv2/opencv.hpp>

void debugDrawCurve(float x, float y);
void eigenAffine(const std::vector<cv::Point2f> &src, const std::vector<cv::Point2f> &dst, float *dis);
void btPreprocess(cv::Mat &src);


#endif