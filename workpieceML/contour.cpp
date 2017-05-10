#include "contour.h"
#include "tools.h"
#include "stdio.h"



void Contours::getContours()
{
    const Mat &binary = image.otsuBinary;
    // imshow("getContours", binary);

    contours.clear();
    item.clear();

    findContours(binary, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    item.resize(contours.size());

    //标志量初始化
    for(int i = 0; i < contours.size(); ++i)
    {
        item[i].flg.lengthflg = false;
        item[i].flg.circleflg = false;
        item[i].flg.pureCirflg= false;
        item[i].flg.matchflg = false;
    }
    insideFlg = false;

    //轮廓长度检验
    maxNumber = 0;
    for(int i = 0; i < contours.size(); ++i)
    {
        if(contours[i].size() > minLength)
        {
            item[i].flg.lengthflg = true;
        }
        if(contours[i].size() > contours[maxNumber].size())
        {
            maxNumber = i;
        }
    }
    
}

void Contours::computeProperty()
{
    image.temp.displayNum = 0;

    for(int i = 0; i < contours.size(); ++i)
    {
        item[i].verifyCircle(contours[i]);
        item[i].verifyPure(image.otsuBinary);
        
        //判断是圆才进行匹配
      //  if(item[i].flg.circleflg)
        {
            item[i].matchRatio = image.temp.matchTemp(image.gray, item[i].center, item[i].radius, item[i].flg.circleflg);
        }
      //  else
        {
      //      item[i].matchRatio = -1;
        }
        //匹配度判断x`
        if(item[i].matchRatio > 0 && item[i].matchRatio < image.temp.matchRatioThresh)
        {
            item[i].flg.matchflg = true;
        }
    }
}


void Contours::display(Mat colorIm, const string name[])
{

    int circleNum = 0, pureNum = 0;

    printf("\n\n");
    printf("contourNum:%d\n", (int)contours.size());
    Mat im(colorIm.size(), CV_8UC3, Scalar(255,255,255));
    // colorIm.copyTo(im);

    // putText(im, "circle", Point(10,60), FONT_HERSHEY_SIMPLEX,
    //         0.4, Scalar(0,0,0));
    for(int i = 0; i < contours.size(); ++i)
    {
        //显示轮廓
        Scalar color(rand()&255, rand()&255, rand()&255);
        drawContours(im, contours, i, color);
        
        printf("size:%3d\t", (int)contours[i].size());
        printf("closeArea:%7.f, circleArea:%8.f, circleRatio:%.2f\t", 
                item[i].closeArea, item[i].circleArea, item[i].scaleArea);
        printf("sflg:%d, circle:%d, pure:%d, match:%d\t", 
                item[i].flg.lengthflg, item[i].flg.circleflg
                , item[i].flg.pureCirflg, item[i].flg.matchflg); 
        
        printf("center x:%d, y:%d\t", (int)item[i].center.x, (int)item[i].center.y);
        
        printf("sumObject:%d\t ratioObj:%0.3f\t", item[i].sumObject, item[i].ratioObject);
        
        printf("match:%1.4f\t", item[i].matchRatio);
        //画out圆
        if(item[i].flg.lengthflg && item[i].flg.circleflg)
        {
            circle(im, item[i].center, item[i].radius, Scalar(255, 0, 0));
            circle(im, item[i].center, item[i].radiusOut, Scalar(0, 255, 0));
            circleNum ++;
        }


        if(item[i].flg.lengthflg && item[i].flg.circleflg 
            && (item[i].flg.matchflg || item[i].flg.pureCirflg))
        {
            rectangle(im, Point(item[i].center.x - item[i].radius, item[i].center.y - item[i].radius)
                        ,Point(item[i].center.x + item[i].radius, item[i].center.y + item[i].radius)
                        ,Scalar(0,0,255));
            printf("draw\t");   
            pureNum ++;
        }
        else
        {
            printf("\t");
        }

        printf("\n");
    }
    
    //画出零件框，即最长轮廓框, //判断矩形是否在视野内
    // RotatedRect rRect = minAreaRect(contours[maxNumber]);
    // Rect bRect = rRect.boundingRect();
    Rect bRect = boundingRect(contours[maxNumber]);
    int interval = 10;
    if(bRect.x - interval > 0 && bRect.y - interval > 0
        && bRect.x + bRect.width + interval < image.gray.cols
        && bRect.y + bRect.height + interval < image.gray.rows)
    {
        insideFlg = true;
        rectangle(im, bRect, Scalar(255, 0, 0));
    }
    char str[10];
 
    sprintf(str, "%d", circleNum);
    putText(im, str, Point(10,50), FONT_HERSHEY_SIMPLEX,
                1.0, Scalar(0,0,0));

    sprintf(str, ":%d", pureNum);
    putText(im, str, Point(30,50), FONT_HERSHEY_SIMPLEX,
                1.0, Scalar(0,0,0));

    imshow(name[CONTOURIMAGE], im);
}

//不知道为什么这里是用引用会报错，加上const后，就没有异常了
void ContourItem::verifyCircle(const vector<Point> contour)
{
    // printf("contour.size:%d\n", (int)contour.size());

    //检验是不是circle
    closeArea = contourArea(contour);
    
    minEnclosingCircle(contour, center, radius);
    circleArea = radius * radius * pi;
    
    scaleArea = circleArea / (closeArea + 1);

    //圆度(面积比)限制、半径限制
    if(scaleArea < 1.3 && scaleArea > 0.7 && radius > 6)
    {
        flg.circleflg = true;
    }
}

void ContourItem::verifyPure(const Mat &binary)
{
     //检验是不是pure circle
    float radiustmp = (radius * 0.4);//外圆
    radiusOut = radiustmp > 5 ? radius + radiustmp : radius + 5; 
    
    int x1 = center.x - radiusOut < 0 ? 0: center.x - radiusOut;
    int y1 = center.y - radiusOut < 0 ? 0: center.y - radiusOut; 
    int x2 = center.x + radiusOut > binary.cols ? binary.cols: center.x + radiusOut;
    int y2 = center.y + radiusOut > binary.rows ? binary.rows: center.y + radiusOut;
    int radiusInner2 = (int)(radius * radius * (1 + 0.2) * (1 + 0.2 ));
    int radiusOuter2 = radiusOut * radiusOut;
    int sumN = 0;  

    sumObject = 1;

    for(int i = y1; i < y2; ++i)
    {
        for(int j = x1; j < x2; ++j)
        {
            sumN ++;
            int length = (i - center.y) * (i - center.y) 
                            + (j - center.x) * (j - center.x);
            if((length > radiusInner2) && length < radiusOuter2
                     && !binary.data[i * binary.cols + j])//?????
            {
                sumObject ++;
            }
        }
    }

    //判断
    ratioObject = sumObject * 1.0f / sumN;
    if((sumObject < 30) && (ratioObject < 0.1))
    {
        flg.pureCirflg = true;
    }
}