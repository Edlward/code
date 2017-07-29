#include <vector>
#include <opencv2/opencv.hpp>
#include "xtofglobal.h"
#include "xtofCalcPyrlk.h"

#define  CV_DESCALE(x,n)     (((x) + (1 << ((n)-1))) >> (n))

using namespace cv;

static void calcSharrDeriv(const cv::Mat& src, cv::Mat& dst);

/****************************************************************************************
 * criteria
 * flags: 是否使用光流的初始值 OPTFLOW_USE_INITIAL_FLOW, 
 		  是否计算误差		OPTFLOW_LK_GET_MIN_EIGENVALS
 * minEigThreshold： 0.002L, 判断所跟踪点的hessian矩阵特征值是否过于小，导致牛顿迭代法失效，退出的阈值
 * maxLevel: 金字塔层数
 *
 * TermCriteria(type,maxCount,epsilon)
 * 		type:终止条件的类型，迭代次数，精度
 *		maxCount:迭代最大次数
 *		epsilon:每一次迭代最少移动的步长，否则退出迭代
*****************************************************************************************/
int xtofCalcPyrlk(vector<Mat> &prevPyr, vector<Mat> &nextPyr,
	InputArray _prevPts, InputOutputArray _nextPts,
	OutputArray _status, OutputArray _err, TermCriteria criteria,
	int flags, double minEigThreshold, int maxLevel)
{
	//static Mat _prevImg;

	Size winSize(WIN_SIZE, WIN_SIZE);

	Mat prevPtsMat = _prevPts.getMat();
	const int derivDepth = DataType<cv::detail::deriv_type>::depth;

	CV_Assert(maxLevel >= 0 && winSize.width > 2 && winSize.height > 2);

	int level = 0, i, npoints;
	npoints = prevPtsMat.checkVector(2, CV_32F, true);

	if (npoints < 1)
	{
		_nextPts.release();
		_status.release();
		_err.release();
		return -1;
	}

	// 不使用初始值
	if (!(flags & OPTFLOW_USE_INITIAL_FLOW))
		_nextPts.create(prevPtsMat.size(), prevPtsMat.type(), -1, true);

	Mat nextPtsMat = _nextPts.getMat();
	CV_Assert(nextPtsMat.checkVector(2, CV_32F, true) == npoints);

	const Point2f* prevPts = (const Point2f*)prevPtsMat.data;
	Point2f* nextPts = (Point2f*)nextPtsMat.data;

	_status.create((int)npoints, 1, CV_8U, -1, true);
	Mat statusMat = _status.getMat(), errMat;
	CV_Assert(statusMat.isContinuous());
	uchar* status = statusMat.data;
	float* err = 0;

	for (i = 0; i < npoints; i++)
		status[i] = true;

	// 有传参应该就是需要
	if (_err.needed())
	{
		_err.create((int)npoints, 1, CV_32F, -1, true);
		errMat = _err.getMat();
		CV_Assert(errMat.isContinuous());
		err = (float*)errMat.data;
	}

	int lvlStep1 = 1;
	int lvlStep2 = 1;

	if ((criteria.type & TermCriteria::COUNT) == 0)
		criteria.maxCount = 30;
	else
		criteria.maxCount = std::min(std::max(criteria.maxCount, 0), 100);
	if ((criteria.type & TermCriteria::EPS) == 0)
		criteria.epsilon = 0.01;
	else
		criteria.epsilon = std::min(std::max(criteria.epsilon, 0.), 10.);
	criteria.epsilon *= criteria.epsilon;

	// dI/dx ~ Ix, dI/dy ~ Iy
	// 预先分配存储梯度图的内存
	Mat derivIBuf;
	if (lvlStep1 == 1)
		derivIBuf.create(prevPyr[0].rows + winSize.height * 2, prevPyr[0].cols + winSize.width * 2, CV_MAKETYPE(derivDepth, prevPyr[0].channels() * 2));

	for (level = maxLevel; level >= 0; level--)
	{
		Mat derivI;
		if (lvlStep1 == 1)
		{
			Size imgSize = prevPyr[level * lvlStep1].size();
			Mat _derivI(imgSize.height + winSize.height * 2, imgSize.width + winSize.width * 2, derivIBuf.type(), derivIBuf.data);
			derivI = _derivI(Rect(winSize.width, winSize.height, imgSize.width, imgSize.height));
			calcSharrDeriv(prevPyr[level * lvlStep1], derivI);//计算梯度图
			copyMakeBorder(derivI, _derivI, winSize.height, winSize.height, winSize.width, 
							winSize.width, BORDER_CONSTANT | BORDER_ISOLATED);//复制边界
		}
		else
			derivI = prevPyr[level * lvlStep1 + 1];

		CV_Assert(prevPyr[level * lvlStep1].size() == nextPyr[level * lvlStep2].size());
		CV_Assert(prevPyr[level * lvlStep1].type() == nextPyr[level * lvlStep2].type());

#ifdef HAVE_TEGRA_OPTIMIZATION
		typedef tegra::LKTrackerInvoker<cv::detail::LKTrackerInvoker> LKTrackerInvoker;
#else
		typedef cv::detail::LKTrackerInvoker LKTrackerInvoker;
#endif
		// parallel_for_使用了并行 计算
		parallel_for_(Range(0, npoints), LKTrackerInvoker(prevPyr[level * lvlStep1], derivI,
			nextPyr[level * lvlStep2], prevPts, nextPts,
			status, err,
			winSize, criteria, level, maxLevel,
			flags, (float)minEigThreshold));
	}
	return 1;
}

cv::detail::LKTrackerInvoker::LKTrackerInvoker(
	const Mat& _prevImg, const Mat& _prevDeriv, const Mat& _nextImg,
	const Point2f* _prevPts, Point2f* _nextPts,
	uchar* _status, float* _err,
	Size _winSize, TermCriteria _criteria,
	int _level, int _maxLevel, int _flags, float _minEigThreshold)
{
	prevImg = &_prevImg;
	prevDeriv = &_prevDeriv;
	nextImg = &_nextImg;
	prevPts = _prevPts;
	nextPts = _nextPts;
	status = _status;
	err = _err;
	winSize = _winSize;
	criteria = _criteria;
	level = _level;
	maxLevel = _maxLevel;
	flags = _flags;
	minEigThreshold = _minEigThreshold;
}

void cv::detail::LKTrackerInvoker::operator()(const Range& range) const
{
	Point2f halfWin((winSize.width - 1)*0.5f, (winSize.height - 1)*0.5f);
	const Mat& I = *prevImg;//第一幅图像
	const Mat& J = *nextImg;//第二幅图像
	const Mat& derivI = *prevDeriv;//第一幅图像梯度，已解算

	int j, cn = I.channels(), cn2 = cn * 2;
	cv::AutoBuffer<deriv_type> _buf(winSize.area()*(cn + cn2));//31*31*3
	int derivDepth = DataType<deriv_type>::depth;

	Mat IWinBuf(winSize, CV_MAKETYPE(derivDepth, cn), (deriv_type*)_buf);//存图像
	Mat derivIWinBuf(winSize, CV_MAKETYPE(derivDepth, cn2), (deriv_type*)_buf + winSize.area()*cn);//存x.y两方向梯度

	// corner循环
	for (int ptidx = range.start; ptidx < range.end; ptidx++)
	{
		Point2f prevPt = prevPts[ptidx] * (float)(1. / (1 << level));
		Point2f nextPt;
		if (level == maxLevel)
		{
			if (flags & OPTFLOW_USE_INITIAL_FLOW)
				nextPt = nextPts[ptidx] * (float)(1. / (1 << level));
			else
				nextPt = prevPt;
		}
		else
			nextPt = nextPts[ptidx] * 2.f;
		nextPts[ptidx] = nextPt;

		Point2i iprevPt, inextPt;
		prevPt -= halfWin; // 角点位于窗口的中心，减去halfWin平移到窗口左上角
		iprevPt.x = cvFloor(prevPt.x); 
		iprevPt.y = cvFloor(prevPt.y);

		if (iprevPt.x < -winSize.width || iprevPt.x >= derivI.cols ||
			iprevPt.y < -winSize.height || iprevPt.y >= derivI.rows)
		{
			if (level == 0)
			{
				if (status)
					status[ptidx] = false;
				if (err)
					err[ptidx] = 0;
			}
			continue;
		}

		float a = prevPt.x - iprevPt.x; // 小数部分
		float b = prevPt.y - iprevPt.y;
		const int W_BITS = 14, W_BITS1 = 14;
		const float FLT_SCALE = 1.f / (1 << 20);

		int iw00 = cvRound((1.f - a)*(1.f - b)*(1 << W_BITS)); // 取最近的整数，预计算，用于插值
		int iw01 = cvRound(a*(1.f - b)*(1 << W_BITS));
		int iw10 = cvRound((1.f - a)*b*(1 << W_BITS));
		int iw11 = (1 << W_BITS) - iw00 - iw01 - iw10;  // 归一化，让四个系数的和为1

		int dstep = (int)(derivI.step / derivI.elemSize1());//224
		int stepI = (int)(I.step / I.elemSize1());
		int stepJ = (int)(J.step / J.elemSize1());
		float A11 = 0, A12 = 0, A22 = 0;  //hessian矩阵的元素



		// extract the patch from the first image, compute covariation matrix of derivatives 
		// hessian matrix
		int x, y;
		for (y = 0; y < winSize.height; y++)
		{
			const uchar* src = (const uchar*)I.data + (y + iprevPt.y)*stepI + iprevPt.x*cn;//原图像
			const deriv_type* dsrc = (const deriv_type*)derivI.data + (y + iprevPt.y)*dstep + iprevPt.x*cn2;//梯度图像

			deriv_type* Iptr = (deriv_type*)(IWinBuf.data + y*IWinBuf.step);
			deriv_type* dIptr = (deriv_type*)(derivIWinBuf.data + y*derivIWinBuf.step);

			x = 0;

			for (; x < winSize.width*cn; x++, dsrc += 2, dIptr += 2)
			{
				int ival = CV_DESCALE(src[x] * iw00 + src[x + cn] * iw01 +
					src[x + stepI] * iw10 + src[x + stepI + cn] * iw11, W_BITS1 - 5);//原图像，图像像素坐标不是整数，所以要插值
				int ixval = CV_DESCALE(dsrc[0] * iw00 + dsrc[cn2] * iw01 +
					dsrc[dstep] * iw10 + dsrc[dstep + cn2] * iw11, W_BITS1);
				int iyval = CV_DESCALE(dsrc[1] * iw00 + dsrc[cn2 + 1] * iw01 + dsrc[dstep + 1] * iw10 +
					dsrc[dstep + cn2 + 1] * iw11, W_BITS1);

				Iptr[x] = (short)ival;
				dIptr[0] = (short)ixval;
				dIptr[1] = (short)iyval;

				A11 += (float)(ixval*ixval);
				A12 += (float)(ixval*iyval);
				A22 += (float)(iyval*iyval);
			}
		}
		//提取窗口循环结束


		A11 *= FLT_SCALE;
		A12 *= FLT_SCALE;
		A22 *= FLT_SCALE;

		float D = A11*A22 - A12*A12;
		float minEig = (A22 + A11 - std::sqrt((A11 - A22)*(A11 - A22) +
			4.f*A12*A12)) / (2 * winSize.width*winSize.height);//计算较小的特征值

		// flags = CV_LKFLOW_GET_MIN_EIGENVALS时计算
		if (err && (flags & CV_LKFLOW_GET_MIN_EIGENVALS) != 0)
		{
			// printf("calc err\n");
			err[ptidx] = (float)minEig;
		}

		if (minEig < minEigThreshold || D < FLT_EPSILON)
		{
			if (level == 0 && status)
				status[ptidx] = false;//判断跟踪失败
			continue;
		}

		D = 1.f / D;

		nextPt -= halfWin;  //角点所在窗口的左上角
		Point2f prevDelta;

		//牛顿迭代法，一般进行了2-3次循环
		for (j = 0; j < criteria.maxCount; j++)
		{
			inextPt.x = cvFloor(nextPt.x);
			inextPt.y = cvFloor(nextPt.y);

			if (inextPt.x < -winSize.width || inextPt.x >= J.cols ||
				inextPt.y < -winSize.height || inextPt.y >= J.rows)
			{
				if (level == 0 && status)
					status[ptidx] = false;
				break;
			}

			a = nextPt.x - inextPt.x;
			b = nextPt.y - inextPt.y;
			iw00 = cvRound((1.f - a)*(1.f - b)*(1 << W_BITS));
			iw01 = cvRound(a*(1.f - b)*(1 << W_BITS));
			iw10 = cvRound((1.f - a)*b*(1 << W_BITS));
			iw11 = (1 << W_BITS) - iw00 - iw01 - iw10;
			float b1 = 0, b2 = 0;

			for (y = 0; y < winSize.height; y++)
			{
				const uchar* Jptr = (const uchar*)J.data + (y + inextPt.y)*stepJ + inextPt.x*cn;
				const deriv_type* Iptr = (const deriv_type*)(IWinBuf.data + y*IWinBuf.step);
				const deriv_type* dIptr = (const deriv_type*)(derivIWinBuf.data + y*derivIWinBuf.step);

				x = 0;


				for (; x < winSize.width*cn; x++, dIptr += 2)
				{
					int diff = CV_DESCALE(Jptr[x] * iw00 + Jptr[x + cn] * iw01 +
						Jptr[x + stepJ] * iw10 + Jptr[x + stepJ + cn] * iw11,
						W_BITS1 - 5) - Iptr[x];//求两幅图像的差分
					b1 += (float)(diff*dIptr[0]);
					b2 += (float)(diff*dIptr[1]);
				}
			}//解算b矩阵

			b1 *= FLT_SCALE;
			b2 *= FLT_SCALE;

			Point2f delta((float)((A12*b2 - A22*b1) * D),
				(float)((A12*b1 - A11*b2) * D));
			// delta = -delta;

			nextPt += delta;
			nextPts[ptidx] = nextPt + halfWin;
			// printf("delta:%f,%f\n", delta.x, delta.y);

			if (delta.ddot(delta) <= criteria.epsilon)//变化量小于一定值时退出
				break;

			if (j > 0 && std::abs(delta.x + prevDelta.x) < 0.01 && // 如果两次的变化量小于0.01，则变化量减小一半
				std::abs(delta.y + prevDelta.y) < 0.01)
			{
				nextPts[ptidx] -= delta*0.5f;
				break;
			}
			// float max_pixel_delta = 2.5f;
			// if(std::abs(delta.x) > max_pixel_delta || std::abs(delta.y) > max_pixel_delta)
			// {
			// 	status[ptidx] = false;
			// }
			
			prevDelta = delta;
		}
		// printf("newton:%d\n", j);
		// 计算法误差及设置status
		// falgs = 0时，计算
		if (status[ptidx] && err && level == 0 && (flags & CV_LKFLOW_GET_MIN_EIGENVALS) == 0)
		{
			// printf("calc err:%d\n", ptidx);
			Point2f nextPoint = nextPts[ptidx] - halfWin;
			Point inextPoint;

			inextPoint.x = cvFloor(nextPoint.x);
			inextPoint.y = cvFloor(nextPoint.y);

			if (inextPoint.x < -winSize.width || inextPoint.x >= J.cols ||
				inextPoint.y < -winSize.height || inextPoint.y >= J.rows)
			{
				if (status)
					status[ptidx] = false;
				continue;
			}

			float aa = nextPoint.x - inextPoint.x;
			float bb = nextPoint.y - inextPoint.y;
			iw00 = cvRound((1.f - aa)*(1.f - bb)*(1 << W_BITS));
			iw01 = cvRound(aa*(1.f - bb)*(1 << W_BITS));
			iw10 = cvRound((1.f - aa)*bb*(1 << W_BITS));
			iw11 = (1 << W_BITS) - iw00 - iw01 - iw10;
			float errval = 0.f;

			for (y = 0; y < winSize.height; y++)
			{
				const uchar* Jptr = (const uchar*)J.data + (y + inextPoint.y)*stepJ + inextPoint.x*cn;
				const deriv_type* Iptr = (const deriv_type*)(IWinBuf.data + y*IWinBuf.step);

				for (x = 0; x < winSize.width*cn; x++)
				{
					int diff = CV_DESCALE(Jptr[x] * iw00 + Jptr[x + cn] * iw01 +
						Jptr[x + stepJ] * iw10 + Jptr[x + stepJ + cn] * iw11,
						W_BITS1 - 5) - Iptr[x];//差分
					errval += std::abs((float)diff);
				}
			}
			err[ptidx] = errval * 1.f / (32 * winSize.width*cn*winSize.height);//误差的计算方式
		}
	}
}


/****************************************************************************************
* Date: 2016/08/22 13:33
*****************************************************************************************/
static void calcSharrDeriv(const cv::Mat& src, cv::Mat& dst)
{
	using namespace cv;
	using cv::detail::deriv_type;
	int rows = src.rows, cols = src.cols, cn = src.channels(), colsn = cols*cn, depth = src.depth();
	CV_Assert(depth == CV_8U);
	dst.create(rows, cols, CV_MAKETYPE(DataType<deriv_type>::depth, cn * 2));



	int x, y, delta = (int)alignSize((cols + 2)*cn, 16);//对齐，使结果为前一个数被后一个数整除的最小数， 舍弃(cols+2)*cn的低4位数
	AutoBuffer<deriv_type> _tempBuf(delta * 2 + 64);
	deriv_type *trow0 = alignPtr(_tempBuf + cn, 16), *trow1 = alignPtr(trow0 + delta, 16);

	//行循环， 这个
	for (y = 0; y < rows; y++)
	{//y=0,srow0=1,srow1=0,srow2=1;  y = 1.srow0=0,srow1=1,srow2=2;
		const uchar* srow0 = src.ptr<uchar>(y > 0 ? y - 1 : rows > 1 ? 1 : 0);//得到当前行的起始地址
		const uchar* srow1 = src.ptr<uchar>(y);
		const uchar* srow2 = src.ptr<uchar>(y < rows - 1 ? y + 1 : rows > 1 ? rows - 2 : 0);
		deriv_type* drow = dst.ptr<deriv_type>(y);

		// do vertical convolution
		x = 0;

		for (; x < colsn; x++)
		{
			int t0 = (srow0[x] + srow2[x]) * 3 + srow1[x] * 10;//连续三行像素
			int t1 = srow2[x] - srow0[x];
			trow0[x] = (deriv_type)t0;
			trow1[x] = (deriv_type)t1;
		}

		// make border
		int x0 = (cols > 1 ? 1 : 0)*cn, x1 = (cols > 1 ? cols - 2 : 0)*cn;
		for (int k = 0; k < cn; k++)
		{
			trow0[-cn + k] = trow0[x0 + k]; trow0[colsn + k] = trow0[x1 + k];//负数行寻址
			trow1[-cn + k] = trow1[x0 + k]; trow1[colsn + k] = trow1[x1 + k];
		}

		// do horizontal convolution, interleave the results and store them to dst
		x = 0;


		for (; x < colsn; x++)
		{
			deriv_type t0 = (deriv_type)(trow0[x + cn] - trow0[x - cn]);
			deriv_type t1 = (deriv_type)((trow1[x + cn] + trow1[x - cn]) * 3 + trow1[x] * 10);
			drow[x * 2] = t0; drow[x * 2 + 1] = t1; // channel中的数是相邻存储的
		}
	}
}
