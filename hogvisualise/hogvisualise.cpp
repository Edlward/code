#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

// HOGDescriptor visual_imagealizer  
// adapted for arbitrary size of feature sets and training images  
Mat get_hogdescriptor_visual_image(Mat& origImg,  
    vector<float>& descriptorValues,  
    Size winSize,  
    Size cellSize,                                     
    int scaleFactor,  //1表示hog可视化图像与原图像一样大小
    double viz_factor)  
{     
    Mat visual_image;  
    resize(origImg, visual_image, Size(origImg.cols*scaleFactor, origImg.rows*scaleFactor));  
  
    int gradientBinSize = 9;  
    // dividing 180° into 9 bins, how large (in rad) is one bin?  
    float radRangeForOneBin = 3.14/(float)gradientBinSize;   
  
    // prepare data structure: 9 orientation / gradient strenghts for each cell  
    int cells_in_x_dir = winSize.width / cellSize.width;  
    int cells_in_y_dir = winSize.height / cellSize.height;  
    int totalnrofcells = cells_in_x_dir * cells_in_y_dir;  
    float*** gradientStrengths = new float**[cells_in_y_dir];  
    int** cellUpdateCounter   = new int*[cells_in_y_dir];  
    for (int y=0; y<cells_in_y_dir; y++)  
        {  
        gradientStrengths[y] = new float*[cells_in_x_dir];  
        cellUpdateCounter[y] = new int[cells_in_x_dir];  
        for (int x=0; x<cells_in_x_dir; x++)  
            {  
            gradientStrengths[y][x] = new float[gradientBinSize];  
            cellUpdateCounter[y][x] = 0;  //这是什么作用
  
            for (int bin=0; bin<gradientBinSize; bin++)  
                gradientStrengths[y][x][bin] = 0.0;  //一个梯度强度由[x,y,bin]决定
            }  
        }  
  
    // nr of blocks = nr of cells - 1  
    // since there is a new block on each cell (overlapping blocks!) but the last one  
    int blocks_in_x_dir = cells_in_x_dir - 1;  
    int blocks_in_y_dir = cells_in_y_dir - 1;  
  
    // compute gradient strengths per cell  
    int descriptorDataIdx = 0;  
    int cellx = 0;  
    int celly = 0;  
  
    for (int blockx=0; blockx<blocks_in_x_dir; blockx++)  
        {  
        for (int blocky=0; blocky<blocks_in_y_dir; blocky++)              
            {  
            // 4 cells per block ...  
            for (int cellNr=0; cellNr<4; cellNr++)  
                {  
                // cell在block中的顺序
                // 0 2
                // 1 3
                // compute corresponding cell nr  
                int cellx = blockx;  
                int celly = blocky;  
                if (cellNr==1) celly++;  
                if (cellNr==2) cellx++;  
                if (cellNr==3)  
                    {  
                    cellx++;  
                    celly++;  
                    }  
  
                for (int bin=0; bin<gradientBinSize; bin++)  
                    {  
                    // hog强度是按block顺序存储的
                    float gradientStrength = descriptorValues[ descriptorDataIdx ];  
                    descriptorDataIdx++;  
  
                    gradientStrengths[celly][cellx][bin] += gradientStrength;  
  
                    } // for (all bins)  
  
  
                // note: overlapping blocks lead to multiple updates of this sum!  
                // we therefore keep track how often a cell was updated,  
                // to compute average gradient strengths  
                // 每一个cell可能被计算多次，但为什么他们的值大小不一样呢？？？？
                cellUpdateCounter[celly][cellx]++;  
  
                } // for (all cells)  
  
  
            } // for (all block x pos)  
        } // for (all block y pos)  
  
  
    // compute average gradient strengths  
    for (int celly=0; celly<cells_in_y_dir; celly++)  
        {  
        for (int cellx=0; cellx<cells_in_x_dir; cellx++)  
            {  
  
            float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];  
  
            // compute average gradient strenghts for each gradient bin direction  
            for (int bin=0; bin<gradientBinSize; bin++)  
                {  
                gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;  
                }  
            }  
        }  
  
  
    cout << "descriptorDataIdx = " << descriptorDataIdx << endl;  
  
    // draw cells  
    for (int celly=0; celly<cells_in_y_dir; celly++)  
        {  
        for (int cellx=0; cellx<cells_in_x_dir; cellx++)  
            {  
            int drawX = cellx * cellSize.width;  
            int drawY = celly * cellSize.height;  
  
            int mx = drawX + cellSize.width/2;  
            int my = drawY + cellSize.height/2;  
  
            rectangle(visual_image,  
                Point(drawX*scaleFactor,drawY*scaleFactor),  
                Point((drawX+cellSize.width)*scaleFactor,  
                (drawY+cellSize.height)*scaleFactor),  
                CV_RGB(100,100,100),  
                1);  
  
            // draw in each cell all 9 gradient strengths  
            for (int bin=0; bin<gradientBinSize; bin++)  
                {  
                float currentGradStrength = gradientStrengths[celly][cellx][bin];  
  
                // no line to draw?  
                if (currentGradStrength==0)  
                    continue;  
  
                float currRad = bin * radRangeForOneBin + radRangeForOneBin/2;  
  
                float dirVecX = cos( currRad );  
                float dirVecY = sin( currRad );  
                float maxVecLen = cellSize.width/2;  
                float scale = viz_factor; // just a visual_imagealization scale,  
                // to see the lines better  
  
                // compute line coordinates  
                float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;  
                float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;  
                float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;  
                float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;  
  
                // draw gradient visual_imagealization  
                line(visual_image,  
                    Point(x1*scaleFactor,y1*scaleFactor),  
                    Point(x2*scaleFactor,y2*scaleFactor),  
                    CV_RGB(0,0,255),  
                    1);  
  
                } // for (all bins)  
  
            } // for (cellx)  
        } // for (celly)  
  
  
    // don't forget to free memory allocated by helper data structures!  
    for (int y=0; y<cells_in_y_dir; y++)  
        {  
        for (int x=0; x<cells_in_x_dir; x++)  
            {  
            delete[] gradientStrengths[y][x];              
            }  
        delete[] gradientStrengths[y];  
        delete[] cellUpdateCounter[y];  
        }  
    delete[] gradientStrengths;  
    delete[] cellUpdateCounter;  
  
    return visual_image;  
  
    }  
  
  
int main()  
{  
      
    HOGDescriptor hog;  
    hog.winSize=Size(80,128);  
    vector<float> des;  
    Mat src = imread("d:/1.jpg");  
    Mat dst ;  
    resize(src,dst,Size(80,128));  
    imshow("src",src);  
    hog.compute(dst,des);  
    Mat d = get_hogdescriptor_visual_image(dst,des,hog.winSize,hog.cellSize,3,2.0);  
    imshow("dst",d);  
    waitKey();  
  
     return 0;  
}  