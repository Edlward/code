#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace cv;
using namespace std;

// int main(int, char**)
// {
//     string win_name = "Live";
//     Mat src;
//     // use default camera as video source
//     namedWindow(win_name, WINDOW_NORMAL);
//     resizeWindow(win_name, 600, 400);

//     VideoCapture cap(0);
//     // check if we succeeded
//     if (!cap.isOpened()) {
//         cerr << "ERROR! Unable to open camera\n";
//         return -1;
//     }
//     // get one frame from camera to know frame size and type
//     cap >> src;
//     // check if we succeeded
//     if (src.empty()) {
//         cerr << "ERROR! blank frame grabbed\n";
//         return -1;
//     }
//     bool isColor = (src.type() == CV_8UC3);
//     //--- INITIALIZE VIDEOWRITER
//     VideoWriter writer;
//     int codec = CV_FOURCC('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
//     double fps = 25.0;                          // framerate of the created video stream
//     string filename = "/home/lxg/codedata/live.avi";             // name of the output video file
//     writer.open(filename, codec, fps, src.size(), isColor);
//     // check if we succeeded
//     if (!writer.isOpened()) {
//         cerr << "Could not open the output video file for write\n";
//         return -1;
//     }
//     //--- GRAB AND WRITE LOOP
//     cout << "Writing videofile: " << filename << endl
//          << "Press any key to terminate" << endl;
//     for (;;)
//     {
//         // check if we succeeded
//         if (!cap.read(src)) {
//             cout << "ERROR! blank frame grabbed\n";
//             break;
//         }
//         // encode the frame into the videofile stream
//         writer.write(src);
//         // show live and wait for a key with timeout long enough to show images
//         imshow(win_name, src);
//         if (waitKey(5) == 27)
//         {
//             cout << "writer end" << endl;
//             break;
//         }
//     }
//     // the videofile will be closed and released automatically in VideoWriter destructor
//     return 0;
// }


#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <fstream>

using namespace std;
using namespace cv;
/*
 * 读取摄像头，保存为视频
 */
int main()
{
    string win_name("camera");
    namedWindow("camera", WINDOW_NORMAL);
    resizeWindow(win_name, 600, 400);
    VideoCapture cap;
    // cap.open(0);
    cap.open("rtsp://admin:admin@192.168.31.67:554/h264/ch1/sub/av_stream");

    if(!cap.isOpened())
    {
        printf("can not open camera\n");
        return -1;
    }
    
    // VideoWriter writer;
    // int codec = CV_FOURCC('M', 'J', 'P', 'G'); 
    // double fps = 25.0;     
    // Size size(cap.get(CAP_PROP_FRAME_WIDTH ), cap.get(CAP_PROP_FRAME_HEIGHT));
    // // writer.open("/home/lxg/codedata/handset/video.avi", codec, fps, size);
    // writer.open("/home/lxg/codedata/counter.avi", codec, fps, size);
    
    // if(!writer.isOpened())
    // {
    //     printf("can not open writer file\n");
    //     return -1;
    // }
    Mat src;

    while(1)
    {
        cap >> src;
        imshow(win_name, src);
        // writer.write(src);
        
        if( waitKey(10) == 27)
        {
            break;
        }
    }
    // writer.release();

}