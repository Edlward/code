#include <stdlib.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "arcsoft_fsdk_face_detection.h"
#include "merror.h"

#define APPID     "6EtR9RZUeC55yirVhnz95fVJbB3YLKGnjyuogDKMLtYY"
#define SDKKEY    "ERmWGed5GcWtsE9WGG22u9d2JTpaqQewRd6vzHyijJmx"

#define INPUT_IMAGE_FORMAT ASVL_PAF_RGB24_B8G8R8
#define WORKBUF_SIZE (40*1024*1024)
#define MAX_FACE_NUM (50)

using namespace cv;

int main(int argc, char **argv)
{
    // 打开摄像头
    VideoCapture cap;
    cap.open(0);
    if(!cap.isOpened())
    {
        fprintf(stderr, "can not open camera\r\n");
        exit(0);
    }
    int image_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int image_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    // face detect sdk init
    MByte *p_workmem = (MByte *)malloc(WORKBUF_SIZE);
    if(p_workmem == nullptr)
    {
        fprintf(stderr, "fail to malloc workbuf\r\n");
        exit(0);
    }

    MHandle h_engine = nullptr;
    int ret = AFD_FSDK_InitialFaceEngine(APPID, SDKKEY, p_workmem, WORKBUF_SIZE,
                                        &h_engine, AFD_FSDK_OPF_0_HIGHER_EXT, 16, MAX_FACE_NUM);
    if(ret != 0)
    {
        fprintf(stderr, "fail to AFD_FSDK_InitialFaceEngine(): 0x%x\r\n", ret);
        free(p_workmem);
        exit(0);
    }

    // image structure init
    ASVLOFFSCREEN input_img = {0};
    int image_buf_len = image_width * image_height * sizeof(uint8_t) * 3;
    input_img.u32PixelArrayFormat = INPUT_IMAGE_FORMAT;
    input_img.i32Width = image_width;
    input_img.i32Height = image_height;
    input_img.ppu8Plane[0] = nullptr;
    if(input_img.u32PixelArrayFormat == ASVL_PAF_RGB24_B8G8R8)
    {
        input_img.pi32Pitch[0] = input_img.i32Width*3;
        input_img.ppu8Plane[0] = (uint8_t *)malloc(image_buf_len);
    }
    else
    {
        fprintf(stderr, "the paragram is designed to open ASVL_PAF_RGB24_B8G8R8\r\n");
        AFD_FSDK_UninitialFaceEngine(h_engine);
        free(p_workmem);
        exit(0);
    }

    Mat src;
    char key = 0;
    while(true)
    {
        // cap image
        cap >> src;
        if(src.empty())
        {
            fprintf(stderr, "camera can continue to read image\r\n");
            break;
        }
        memcpy(input_img.ppu8Plane[0], src.data, image_buf_len);

        // begine to detect
        double t = (double)cv::getTickCount();
        LPAFD_FSDK_FACERES face_result;
        ret = AFD_FSDK_StillImageFaceDetection(h_engine, &input_img, &face_result);
        if(ret != 0)
        {
            fprintf(stderr, "fail to AFD_FSDK_StillImageFaceDetection(): 0x%x\r\n", ret);
            break;
        }
        printf("time consumed(s): %f\t", ((double)cv::getTickCount()- t)/cv::getTickFrequency());
        
        // show detected face
        for(int i = 0; i < face_result->nFace; ++i)
        {
            printf("face %d:(%d,%d,%d,%d)\t angle:%d\n", i+1,
                    face_result->rcFace[i].left, face_result->rcFace[i].top,
                    face_result->rcFace[i].right, face_result->rcFace[i].bottom,
                    face_result->lfaceOrient[i]);
            
            Rect rect(face_result->rcFace[i].left, face_result->rcFace[i].top,
                        face_result->rcFace[i].right - face_result->rcFace[i].left,
                        face_result->rcFace[i].bottom - face_result->rcFace[i].top);
            rectangle(src, rect, Scalar(0,0,255), 2);
        }
        imshow("face", src);
        key = waitKey(30);
        if(key == 27)
        {
            break;
        }
    }
    
    free(input_img.ppu8Plane[0]);
    AFD_FSDK_UninitialFaceEngine(h_engine);
    free(p_workmem);
    
    return 0;
}