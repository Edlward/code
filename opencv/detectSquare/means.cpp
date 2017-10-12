#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

enum Direction
{
    LEFT,
    RIGHT
};

const float uniform = 0.003921;

int horizontalProjectMat(const Mat &im, int square_size)
{
    int height = im.rows;
    int width = im.cols;
    
    // 水平投影
    float *project_arr = new float[height];
    memset(project_arr, 0, height * sizeof(float));
    for(int i = 0; i < height; ++i)
    {
        const uchar *ptr = im.ptr<uchar>(i); 
        for(int j = 0; j < width; ++j)
        {
            project_arr[i] += *ptr * uniform;
            ptr++;
        }
    }

    // 计算总量，并计算没行所占比例， 计算期望
    float sum = 1.f, center = 0.f;
    for(int i = 0; i < height; ++i)
    {
        sum += project_arr[i];
    }
    for(int i = 0; i < height; ++i)
    {
        center += project_arr[i]*i / sum;
    }

    if(sum < square_size * square_size / 10)
    {
        center = 0;
    }

    // 画图显示
    Mat back = Mat::ones(height, width, CV_8UC1) * 255;
    for(int j = 0; j < height; ++j)
    {
        uchar *ptr = back.ptr<uchar>(j);
        for(int i = 0; i < floor(project_arr[j]); ++i)
        {
            *ptr = 0;
            ++ptr;
        }
    }
    // 画期望值曲线
    uchar *ptr = back.ptr<uchar>(center);
    for(int i = 0; i < width; ++i)
    {
        *ptr = 0;
        ptr++;
    }

    imshow("back_horizontal", back);
    delete [] project_arr;
    return floor(center);
}

void verticalProjectMat(const Mat &im, int dir, int square_size, vector<int> &coordx)
{
    coordx.clear();

    int height = im.rows;
    int width = im.cols;

    float *project_arr = new float[width];
    memset(project_arr, 0, width * sizeof(float));//初始化数组

    for(int i = 0; i < height; ++i)
    {
        const uchar *ptr = im.ptr<uchar>(i);
        for(int j = 0; j < width; ++j, ++ptr)
        {
            project_arr[j] += *ptr * uniform;
        }
    }

    Mat back = Mat::ones(height, width, CV_8UC1) * 255;
    for(int j = 0; j < width; ++j)
    {
        for(int i = 0; i < floor(project_arr[j]); ++i)
        {
            back.at<uchar>(height - 1 - i, j) = 0;
        }
    }

    float *project_arr_sum = new float[width];
    memset(project_arr_sum, 0, width * sizeof(float));
    project_arr_sum[0] = project_arr[0];
    for(int i = 1; i < width; ++i)
    {
        project_arr_sum[i] = project_arr_sum[i-1] + project_arr[i];
    }

    float thresh_small = 200;
    float thresh_big = 100;
    int judge_width = 4;
    bool step_flg = true;
    int square_num = 0;

    // if(dir == LEFT)
    // {
    //     int i = 0;
    //     for(; i < width - judge_width; ++i) //跳过第一个
    //     {

    //     }
    // }
    if(dir == LEFT)
    {
        for(int i = 0; i < width - judge_width; ++i)
        {
            if(step_flg && (project_arr_sum[i+judge_width] - project_arr_sum[i] > thresh_small))
            {
                coordx.push_back(i);
                i += 0.8*square_size;
                square_num ++;
                step_flg = false;
                if(square_num == 3)
                {
                    break;
                }
            }
            else if(!step_flg && (project_arr_sum[i+judge_width] - project_arr_sum[i] < thresh_big))
            {
                coordx.push_back(i);
                step_flg = true;
            }
            else if(!step_flg && (i - coordx[square_num*2-2]) > 1.5*square_size)
            {
                coordx.push_back(coordx[square_num*2-2] + square_size);
                i += coordx[square_num*2-2] + square_size;
                step_flg = true;
            }
        }
    }
    else if(dir == RIGHT)
    {
        for(int i = width - 1; i > 0; --i)
        {
            if(step_flg && (project_arr_sum[i] - project_arr_sum[i-judge_width] > thresh_small))
            {
                coordx.push_back(i);
                i -= 0.8*square_size;
                square_num++;
                step_flg = false;
                if(square_num == 3)
                {
                    break;
                }
            }
            else if(!step_flg && (project_arr_sum[i] - project_arr_sum[i-judge_width] < thresh_big))
            {
                coordx.push_back(i);
                step_flg = true;
            }
            else if(!step_flg && (coordx[square_num*2-2] - i) > 1.5 * square_size)
            {
                coordx.push_back(coordx[square_num*2-2] - square_size);
                i -= coordx[square_num*2-2] - square_size;
                step_flg = true;
            }
        }
    }

    
    imshow("vertical", back);
    delete [] project_arr;
    delete [] project_arr_sum;
}

int main(int argc, char **argv)
{
    VideoCapture cap;
    // Rect2d rect_roi(600, 180, 600, 250);  // s1.avi
    // cap.open("/home/lxg/codedata/square/s4.avi");

    Rect2d rect_roi(0, 30, 700, 250);  // s2.avi
    cap.open("/home/lxg/codedata/square/s2.avi");
    
    cout << "height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
    // temp 155, 132, 119
    Mat temp = imread("/home/lxg/codedata/square/temp.png"); //temp1 47, 43, 38
    Scalar rgb = sum(temp) / (temp.cols * temp.rows);
    cout << "rgb" << rgb << endl;

    Mat temp2 = imread("/home/lxg/codedata/square/temp2.png"); // temp2 241, 219, 215
    Scalar rgb_track = sum(temp2) / (temp2.cols * temp2.rows);
    cout << "rgb_track " << rgb_track << endl;

    Mat im, roi, gray, binary, vertical_im, track_im, show_im, yuv;

    Rect2d rect_track(200, 180, 450, 400);
    // Rect2d rect_roi(0, 30, 700, 250);  //s2
    // Rect2d rect_roi(300, 180, 800, 400);
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    int centery = 0, centery_track = 0;
    int square_size = 100;
    int thresh = 20;
    // int dir = LEFT;
    int dir = RIGHT;
    
    vector<int> coordx;
    char key = 0;

    while(1)
    {
        cap >> im;
        im.copyTo(show_im);
        cvtColor(im, gray, CV_RGB2GRAY);

        // track square
        // rectangle(show_im, rect_track, Scalar(255,0,0), 2);
        // roi = im(rect_track);
        // inRange(roi, Scalar(rgb_track[0]-thresh, rgb_track[1]-thresh, rgb_track[2]-thresh), Scalar(rgb_track[0]+thresh, rgb_track[1]+thresh, rgb_track[2]+thresh), binary);
        
        // centery_track = horizontalProjectMat(binary, square_size);
        // centery_track = std::max(centery_track, square_size/2);
        // centery_track = std::min(centery_track, (int)floor(rect_track.height-square_size/2));
        // line(show_im, Point(rect_track.x, rect_track.y+centery_track-square_size/2), Point(rect_track.x+rect_track.width,rect_track.y+centery_track-square_size/2), Scalar(255,0,0),1);
        // line(show_im, Point(rect_track.x, rect_track.y+centery_track+square_size/2), Point(rect_track.x+rect_track.width,rect_track.y+centery_track+square_size/2), Scalar(255,0,0),1);

        // cout << "  centery_track " << centery_track << endl;
        // vertical_im = binary(Rect(0, centery_track-square_size/2, rect_track.width, square_size));
        // verticalProjectMat(vertical_im, dir, coordx, square_size);
        // line(show_im, Point(coordx[0]+rect_track.x, rect_track.y), Point(coordx[0]+rect_track.x, rect_track.y+rect_track.height), Scalar(255,0,0));
        // line(show_im, Point(coordx[1]+rect_track.x, rect_track.y), Point(coordx[1]+rect_track.x, rect_track.y+rect_track.height), Scalar(255,0,0));
        

                
        // detect square
        // rectangle(show_im, rect_roi, Scalar(0,0,255), 2);
        // roi = im(rect_roi);  //二值化后是(0,255), 单通道
        // inRange(roi, Scalar(rgb[0]-thresh, rgb[1]-thresh, rgb[2]-thresh), Scalar(rgb[0]+thresh, rgb[1]+thresh, rgb[2]+thresh), binary);

        // centery = horizontalProjectMat(binary, square_size);
        // line(show_im, Point(rect_roi.x, rect_roi.y+centery-square_size/2), Point(rect_roi.x+rect_roi.width,rect_roi.y+centery-square_size/2), Scalar(0,0,255),1);
        // line(show_im, Point(rect_roi.x, rect_roi.y+centery+square_size/2), Point(rect_roi.x+rect_roi.width,rect_roi.y+centery+square_size/2), Scalar(0,0,255),1);
        // cout << "centery " << centery;
    
        // vertical_im = binary(Rect(0,centery-square_size/2, rect_roi.width, square_size));
        // verticalProjectMat(vertical_im, dir, coordx, square_size);
        // line(show_im, Point(coordx[0]+rect_roi.x, rect_roi.y), Point(coordx[0]+rect_roi.x, rect_roi.y+rect_roi.height), Scalar(0,0,255));
        // line(show_im, Point(coordx[1]+rect_roi.x, rect_roi.y), Point(coordx[1]+rect_roi.x, rect_roi.y+rect_roi.height), Scalar(0,0,255));
        
        // detect two
        // roi = im(rect_roi);  //二值化后是(0,255), 单通道
        // inRange(roi, Scalar(rgb[0]-thresh, rgb[1]-thresh, rgb[2]-thresh), Scalar(rgb[0]+thresh, rgb[1]+thresh, rgb[2]+thresh), binary);
        
        cvtColor(im, yuv, CV_BGR2Luv);
        roi = yuv(rect_roi);
        inRange(roi, Scalar(22, 84, 109), Scalar(223, 96, 137), binary);

        imshow("rgb_similarity", binary);

        centery = horizontalProjectMat(binary, square_size);
        line(show_im, Point(rect_roi.x, rect_roi.y+centery-square_size/2), Point(rect_roi.x+rect_roi.width,rect_roi.y+centery-square_size/2), Scalar(0,0,255),1);
        line(show_im, Point(rect_roi.x, rect_roi.y+centery+square_size/2), Point(rect_roi.x+rect_roi.width,rect_roi.y+centery+square_size/2), Scalar(0,0,255),1);
        cout << "centery " << centery;

        vertical_im = binary(Rect(0,centery-square_size/2, rect_roi.width, square_size));
        verticalProjectMat(vertical_im, dir, square_size, coordx);
        for(size_t i = 0; i < coordx.size()/2; ++i)
        {
            Scalar color;
            switch(i%3)
            {
            case (0):
                color = Scalar(0, 0, 255);
                break;
            case (1):
                color = Scalar(255,0,0);
                break;
            case (2):
                color = Scalar(0,255,0);
                break;
            }
            line(show_im, Point(coordx[i*2]+rect_roi.x, rect_roi.y), Point(coordx[i*2]+rect_roi.x, rect_roi.y+rect_roi.height), color);
            line(show_im, Point(coordx[i*2+1]+rect_roi.x, rect_roi.y), Point(coordx[i*2+1]+rect_roi.x, rect_roi.y+rect_roi.height), color);
        }

        // otsu
        // vertical_im = gray(Rect(rect_roi.x, rect_roi.y + centery-square_size/2, rect_roi.width, square_size));
        // threshold(vertical_im, binary, 0, 255, THRESH_OTSU);
        // imshow("otsu", binary);
        
        // vertical sobel
        // Mat edge_sobel, edge_binary;
        // Sobel(gray(rect_roi), edge_sobel, CV_8U, 0, 1);
        // imshow("sobel", edge_sobel);
        // threshold(edge_sobel, edge_binary, 50, 255, THRESH_BINARY);
        // imshow("edge_binay", edge_binary);
        
        // vector<vector<Point> > contours;
        // findContours(edge_binary, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
        // for(size_t i = 0; i < contours.size(); ++i)
        // {
        //     if(contours[i].size() < 100)
        //     {
        //         continue;
        //     }
        //     Scalar color(rand()&255, rand()&255, rand()&255);
        //     drawContours(vertical_im, contours, i, color, 2, 8);
        // }

        
        rectangle(show_im, rect_roi, Scalar(0,0,255), 2);
        imshow("src", show_im);
        imshow("vertical_im", vertical_im);

        cout << endl;
        if((key = waitKey(1)) == 27)
        {
            break;
        }
        else if(key == 's')
        {
            waitKey(0);
        }

    }
}