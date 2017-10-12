#include <opencv2/opencv.hpp>
#include <vector>
// mentohust -uM201672461 -p033058

using namespace std;
using namespace cv;

float uniform = 0.003921;

void horizontalProjectMat(const Mat &im)
{
    int height = im.rows;
    int width = im.cols;
    
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

    // otsu
    int threshold;
    float w0, w1, u0tmp, u1tmp, u0, u1, u, delta_tmp, delta_max = 0;
    float sum = 0, prob;
    for(int i = 0; i < height; ++i)
    {
        sum += project_arr[i];
    }
    for(int i = 0; i < height; ++i)
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = delta_tmp = 0;
        for(int j = 0; j < height; ++j)
        {
            prob = project_arr[j] / sum; // normalised to 1
            if(j <= i)
            {
                w0 += prob; 
                u0tmp += j * prob;   
            }
            else
            {
                w1 += prob;
                u1tmp += j * prob;
            }
        }
        u0 = u0tmp / w0;
        u1 = u1tmp / w1;
        u = u0tmp + u1tmp;
        delta_tmp = w0 * pow((u0-u), 2) + w1 * pow((u1-u), 2);
        if(delta_tmp > delta_max)
        {
            delta_max = delta_tmp;
            threshold = i;
        }
    }

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

    uchar *ptr = back.ptr<uchar>(threshold);
    for(int i = 0; i < width; ++i)
    {
        *ptr = 0;
        ptr++;
    }

    imshow("back_horizontal", back);
    delete [] project_arr;
}

void verticalProjectMat(const Mat &im)
{
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

    imshow("vertical", back);
    delete [] project_arr;
    // return back;
}

int main(int argc, char **argv)
{
    VideoCapture cap;
    cap.open("/home/lxg/codedata/square/s2.avi");
    cout << "height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;

    Mat im, roi, gray, binary;
    // Rect2d rect_roi(650, 180, 600, 250);
    Rect2d rect_roi(0, 30, 700, 250);  //s2
    

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    while(1)
    {
        cap >> im;
        cvtColor(im, gray, CV_RGB2GRAY);

        roi = gray(rect_roi);
        verticalProjectMat(roi);
        horizontalProjectMat(roi);
        rectangle(im, rect_roi, Scalar(0,0,255), 2);

        // threshold(roi, binary, 0, 255, THRESH_OTSU);
        // imshow("binary", binary);
        // findContours(binary, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
        // for(size_t i = 0; i < contours.size(); ++i)
        // {
        //     Scalar color(rand()&255, rand()&255, rand()&255);
        //     drawContours(im(rect_roi), contours, i, color, 2, 8);
        // }
        

        imshow("square", im);
        if(waitKey(10) == 27)
        {
            break;
        }

    }
}