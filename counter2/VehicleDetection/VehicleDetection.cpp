#include "stdafx.h"
#include <cvblob.h>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <memory>
#include <iostream>

using namespace cv;
using namespace cvb;
using namespace std;


void processVideo();

long int inputNum  = 0;
long int outputNum = 0;


//shadow parameters
double th1 =0.2;
double th2 =0.6;
double th3 =1.5;
double th4 =20;

double beta1  0.4;
double beta2  0.6;
double Th     0.5; 
double Ts     0.l;

//line parameters
int upLine   =110;
int downLine =130;

//interest region
int widthx1 =80;
int widthx2 =330;
int heighty1 =40;
int heighty2 =240;

//shouldWidth set
int MultiPeople1 =120;
int MultiPeople2 =240;
int MultiPeople3 =300;

//blob area threshod
int small=2000;
int big  =20000;

//




int main(int argc, char* argv[])
{
// 	if (argc != 2)
// 	{
// 		cout << "Usage: VehicleDetection.exe videofile" << endl;
// 		return 1;
// 	}
	processVideo();
	cvDestroyAllWindows();
	return 0;
}

//void detectModule(Mat src,Mat bg,Mat fore,int i,int j)
//{
       //将原始图像转换到HSV空间
       //HSV各分量范围H从0到180，S为0到255，V为0到255
//	Mat src_HSV;
//	Mat bg_HSV;
	//转换
 //	cvtColor(src,src_HSV,COLOR_BGR2HSV);
//	cvtColor(bg, bg_HSV, COLOR_BGR2HSV);

//	double TempH =0.0;
//	double TempS =0.0;
//	double TempV =0.0;
	
//	double Ih =0.0;
//	double Is =0.0;
//	double Iv =0.0;
//
//	double Bh = bg_HSV.at<Vec3b>(i,j)[0]*2.0;
//	double Bs = bg_HSV.at<Vec3b>(i,j)[1]/255.0;
//	double Bv = bg_HSV.at<Vec3b>(i,j)[2]/255.0;

	//在某一个区域内判断
//	for(int m=-1;m<=1;m++)
//	   {
//		for(int n=-1;n<=1;n++)
//		{
		//求取三个区域HSV三个通道的平均值
		//累加H通道,转换为真正的HSV空间
//		TempH += src_HSV.at<Vec3b>(i + m, j + n)[0]*2.0;
		//累加S通道
//		TempS += src_HSV.at<Vec3b>(i + m, j + n)[1]/255.0;
		//累计V通道
//		TempV += src_HSV.at<Vec3b>(i + m, j + n)[2]/255.0;
//		}
//	   }

//	Ih = TempH / 9.0;
//	Is = TempS / 9.0;
//	Iv = TempV / 9.0;


	//判断,若为阴影，则将该点修改为背景点
//	if (Iv / Bv >= beta1 && Iv / Bv <= beta2 && (Is - Bs) <= Ts && fabs(Ih - Bh) <= Th){
//		fore.at<uchar>(i, j) = 0;
//	}
//	else{
//		fore.at<uchar>(i, j) = 255;
//	}

//}

void detectModule(Mat src,Mat bg,Mat fore,int i,int j)
{
	//yuanshi tuxiang 
	int srcR = bg.at<Vec3b>(i,j)[0];
	int srcG = bg.at<Vec3b>(i,j)[1];
	int srcB = bg.at<Vec3b>(i,j)[2];
	double It = (srcR+srcG+srcB)/3;
      
	double rt = srcR/((srcR+srcG+srcB)*1.0);
	double gt = srcG/((srcR+srcG+srcB)*1.0);

         //计算区块三通道均值亮度和
        int  bgR = src.at<Vec3b>(i,j)[0];
        int  bgG = src.at<Vec3b>(i,j)[1]; 
        int  bgB = src.at<Vec3b>(i,j)[2]; 
        double Ib = (bgR+bgG+bgB)/3;

	double rb = bgR/((bgR+bgG+bgB)*1.0);
	double gb = bgG/((bgR+bgG+bgB)*1.0);


         //该点的亮度值如果与周围点的亮度值接近的话，则认为该点是阴影点
        if(  fabs(rt-rb)<=th1 && fabs(gt-gb)<th1 && It/Ib>th2 && It/Ib<=th3 && fabs(It-Ib)<th4 ) 
        {
                fore.at<uchar>(i,j) = 0;
        }
        else
	{
		fore.at<uchar>(i,j) =255;
	}
}

void detectShadow(Mat src,Mat bg,Mat fore)
{
        //Temp.at<uchar>(i + 0, j + 0) = 255;
        //前景点与原始背景区域周围点的亮度信息相差很大，而阴影点则基本一致，因 $
        int width = src.cols;
        int height= src.rows;
	 

        for(int i=0;i<height;i++)
        {
                for(int j=0;j<width;j++)
                {
                        int foreValue = fore.at<uchar>(i,j);
                        if( foreValue == 255 )
                        {
                                //只处理前景点
                                detectModule(src,bg,fore,i,j);
                        }
                }
        }
        cout<<"check4"<<endl;
}



Mat rotateImage(Mat img,int degree)
{
   	degree = -degree;
	double angle   = degree *CV_PI/180;
        double a   = sin(angle),b=cos(angle);
        int width  = img.cols;
        int height = img.rows;     
        float map[6];
        Mat map_matrix = Mat(2,3,CV_32F,map);
        CvPoint2D32f center = cvPoint2D32f(width/2,height/2);
        CvMat map_matrix2 = map_matrix;
        cv2DRotationMatrix(center,degree,1.0,&map_matrix2);
        Mat img_rotate;
        warpAffine(img,img_rotate,map_matrix,Size(width,height),1,0,0);
        return img_rotate;
}


void Sign(Mat src,int row1,int row2)
{
	//在原始图像中画图和标注
	//进入的线
	line(src, Point(0,upLine),Point(352,upLine),Scalar(0,0,255),2);
	//出去的线
	line(src, Point(0,downLine), Point(352, downLine), Scalar(0, 255, 0), 2);

	string Input  = to_string(inputNum);
	string Output = to_string(outputNum);

	string string1 = "InputPeople";
	string string2 = "OutputPeople";

	string1 = string1 + "_" + Input;
	string2 = string2 + "_" + Output;

	//绘制统计数目
	putText(src, string1, Point(0, int(src.rows*0.2)),
		    CV_FONT_HERSHEY_COMPLEX, src.cols / 200, cvScalar(0, 0, 255, 0),2);
	putText(src, string2, Point(0, int(src.rows*0.5)),
		    CV_FONT_HERSHEY_COMPLEX, src.cols / 200, cvScalar(0,255, 0, 0),2); 

}

/***************************************************************************************************************
Unger平滑的思想是检查3*3窗口内图像结构的细节，而不是简单的统计取值为1的像素数。
其中g为当前点，P0 ～P7为其八个邻接点，Unger平滑准则为：
当 g=1即g为白像素时，如果①P0、P1、P2中至少有一个等于1，同时P4、P5、P6中至少有一个也等于1，
或者②P2、P3、P4中至少有一个等于1，同时P5、P7、P0中至少有一个也等于1，则g不变；否则，g=0．
当 g=0即g为黑象素时，如果P0、P2、P4、P6中至少有三个元素等于1，则g改为1；否则，g不变
****************************************************************************************************************/
void UngerFilter(Mat src)
{
	Mat Temp = src * 0;
	for (int i = 1; i<src.rows-1; i++)
	{
		for (int j = 1; j<src.cols-1; j++)
		{
			
			int g  = src.at<uchar>(i + 0, j + 0); 
			int P0 = src.at<uchar>(i + 1, j + 0);
			int P1 = src.at<uchar>(i + 1, j - 1);
			int P2 = src.at<uchar>(i + 0, j - 1);
			int P3 = src.at<uchar>(i - 1, j - 1);
			int P4 = src.at<uchar>(i - 1, j + 0);
			int P5 = src.at<uchar>(i - 1, j + 1);
			int P6 = src.at<uchar>(i + 0, j + 1);
			int P7 = src.at<uchar>(i + 1, j + 1);

			if (g == 255)
			{
				if (((P0 == 255 || P1 == 255 || P2 == 255) && (P4 == 255 || P5 == 255 || P6 == 255)) ||
					((P2 == 255 || P3 == 255 || P4 == 255) && (P5 == 255 || P7 == 255 || P0 == 255)))
				{
					Temp.at<uchar>(i + 0, j + 0) = 255;
				}
				else
				{
					Temp.at<uchar>(i + 0, j + 0) = 0;
				}
			}
			else
			{
				if (P0 == 255 || P2 == 255 || P4 == 255 || P6 == 255)
				{
					Temp.at<uchar>(i + 0, j + 0) = 255;
				}
				else
				{
					Temp.at<uchar>(i + 0, j + 0) = 0;
				}
			}
		}
	}
	src = Temp;
}

void processVideo()
{
	Mat frame;
	Mat Tempframe; // current frame
	Mat fgMaskMOG2; // fg mask fg mask generated by MOG2 method
	Mat bgImg; // background
	Ptr<BackgroundSubtractorMOG2> pMOG2 = createBackgroundSubtractorMOG2(200, 36.0, false); // MOG2 Background subtractor

	//int degree =30;
	cout<<"check1"<<endl;
	VideoCapture capture;
	// capture.open("/home/lxg/codedata/walmat.mp4");
	capture.open("rtsp://admin:admin@192.168.31.67:554/h264/ch1/sub/av_stream");
	cout<<"check2"<<endl;
	
	//save the video

	while (true)
	{
		//walmat.mp4这个视频尺寸为800*600，有效探测区域可以认定为Rect（270,200,300,200）
		
		if (!capture.isOpened())
		{
			cerr << "Unable to open video file: "  << endl;
			return;
		}

		int width  = (int)capture.get(CV_CAP_PROP_FRAME_WIDTH);
		int height = (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT);

		cout<<width<<endl;
		cout<<height<<endl;
		unique_ptr<IplImage, void(*)(IplImage*)> labelImg(cvCreateImage(cvSize(width, height), IPL_DEPTH_LABEL, 1),
			[](IplImage* p){ cvReleaseImage(&p); });
		CvBlobs blobs;
		CvTracks tracks;

		///每一帧开始检测跟踪
		while (true)
		{
			// read input data. ESC or 'q' for quitting
			int key = waitKey(1);
			if (key == 'q' || key == 27)
				return;
			if (!capture.read(Tempframe))
				break;
			
			//设置比实际统计区域大
			frame = Tempframe(Range(heighty1, heighty2), Range(widthx1, widthx2));
			//frame = rotateImage(Tempframe,degree);

			// update background
			pMOG2->apply(frame, fgMaskMOG2);
			pMOG2->getBackgroundImage(bgImg);
			// imshow("背景", bgImg);
			imshow("Original mask1", fgMaskMOG2);
			 cout<<"xz"<<endl;
                        detectShadow(frame,bgImg,fgMaskMOG2);
			// post process
			 cout<<"zqq"<<endl;
                        imshow("AfterShadow", fgMaskMOG2);

			// urger 平滑
			UngerFilter(fgMaskMOG2);
			//medianBlur(fgMaskMOG2, fgMaskMOG2, 5);
		    //imshow("AfterShadow", fgMaskMOG2);
			morphologyEx(fgMaskMOG2, fgMaskMOG2, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(5, 5))); // fill black holes
			//morphologyEx(fgMaskMOG2, fgMaskMOG2, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(5, 5))); // fill white holes
			// imshow("morphologyEx", fgMaskMOG2);

			unsigned int Label;
			// track
			//轮廓总数
			IplImage img(fgMaskMOG2);
			cvLabel(&img, labelImg.get(), blobs);

			//将轮廓面积限制在一定区间
			cvFilterByArea(blobs, small, big);
			//输出Blob总数看看,经过滤波后小块非常少，比较适合跟踪了
			//int sum = blobs.size();
			//cout << "blobNum" << sum << endl;
			//打印函数，只是打印跟踪信息
			//更新追踪
			//shouldWidth set
			//int MultiPeople1 =120;
			//int MultiPeople2 =240;
			//int MultiPeople3 =300;

			//line parameters
            //int upLine   =110;
            //int downLine =130;
			cvUpdateTracks(upLine,downLine,MultiPeople1, MultiPeople2, MultiPeople3,inputNum, outputNum, blobs, tracks, 10, 90, 30);
		
			bool sign    = true;
			int  NewCome = 0;
			if ( NewCome = tracks.size() >0 )
			{
				sign = false;
			}
			//IplImage f1(frame);
			// cvRenderTracks(tracks, &IplImage(frame), &IplImage(frame));
			//cvRenderTracks(tracks, &f1, &f1);
			
			Sign(Tempframe, 50,250);

			imshow("MonitorSolution", Tempframe);
			if (!capture.read(Tempframe))
			break;

			// show
			//imshow("Frame", frame);

			key = waitKey(1);
		}
	}
}
