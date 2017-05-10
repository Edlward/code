#include <string>
#include <stdio.h>
#include "image.h"
#include "tools.h"
#include "contour.h"
#include "temp.h"

int main(int argc, char **argv)
{
    std::string windowsName[6];
    init(windowsName);

    
    VideoCapture cap;
    openCamera(cap);

    CImage image;
    image.temp.tempInit();

    while(1)
    {
        cap >> image.src;
        image.imageConvert();

        // imshow(windowsName[ADABINARY], image.adaBinary);
        imshow(windowsName[SRCIMAGE], image.src);
        imshow(windowsName[GRAYIMAGE], image.gray);
        imshow(windowsName[OTSUBINARY], image.otsuBinary);

        Contours contour(image);
        
        contour.getContours();
        contour.computeProperty();
        contour.display(image.src, windowsName);

        char key = waitKey(10);

        if(key == 27)
        {
            break;
        }
        else if(key == 's')
        {
            waitKey(0);
        }
    }

    destroyAllWindows();
}

