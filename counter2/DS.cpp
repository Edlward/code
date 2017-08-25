void detectModule(Mat src,Mat fore,int i,int j,int Threshod)
{
	long int SumRGB = 0;
	int Average = 0;
	int R,G,B;

	int RGB = src.at<Vec3b>(i,j)[0]+src.at<Vec3b>(i,j)[1]+src.at<Vec3b>(i,j)[2];
	for(int m=-1;m<=1;m++)
	{
		for(int n=-1;n<=1;n++)
		{
			//计算区块三通道均值亮度和
			R = src.at<Vec3b>(i+m,j+n)[0];
			G = src.at<Vec3b>(i+m,j+n)[1]; 
			B = src.at<Vec3b>(i+m,j+n)[2]; 
			SumRGB += R+G+B;
		}
	}
	//均值
	Average = int(SumRGB/9);
    //该点的亮度值如果与周围点的亮度值接近的话，则认为该点是阴影点
	if( abs (RGB-Average)< Threshod ) 
	{
		fore.at<uchar>(i,j) = 0;
	}
}

void detectShadow(Mat src,Mat fore,int Threshod)
{
	//Temp.at<uchar>(i + 0, j + 0) = 255;
	//前景点与原始背景区域周围点的亮度信息相差很大，而阴影点则基本一致，因为阴影是投影到背景表面上的，两者颜色亮度较为一致
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
				detectModule(src,fore,i,j,Threshod);
			}
		}
	}
}


void Sign(Mat src,int row1,int row2)
{
	//frame = Tempframe(Range(40, 240), Range(80, 330));
	//在原始图像中画图和标注
	//共有三条线
	//上、下、竖直线，
	//上
	line(src, Point(0,90), Point(352, 90),  Scalar(0, 0, 255), 2);
	//下
	line(src, Point(0,120), Point(352, 120), Scalar(0, 255, 0), 2);
	//竖线
	//line(src, Point(330,50),  Point(330, 270), Scalar(255,0, 0), 2);

	int line1 = 200;
	int line2 = 400;

	string Up   = to_string(UpNum);  
	string Down = to_string(DownNum);
	//string Right= to_string(RightNum);


	string string1 = "UpPeople";
	string string2 = "DownPeople";
	//string string3 = "RightPeople";

	string1 = string1 + "_" + Up;
	string2 = string2 + "_" + Down;
	//string3 = string3 + "_" + Right;


	//绘制统计数目
	putText(src, string1, Point(0, int(src.rows*0.2)),
		    CV_FONT_HERSHEY_COMPLEX, src.cols / 300, cvScalar(0, 0, 255, 0),2);
	putText(src, string2, Point(0, int(src.rows*0.5)),
		    CV_FONT_HERSHEY_COMPLEX, src.cols / 300, cvScalar(0,255, 0, 0),2); 
	//putText(src, string3, Point(0, int(src.rows*0.5)),
		   // CV_FONT_HERSHEY_COMPLEX, src.cols / 300, cvScalar(0,255, 0, 0),2); 

}
