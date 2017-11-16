#include "faceDetectHong.h"

#define APPID     "6EtR9RZUeC55yirVhnz95fVJbB3YLKGnjyuogDKMLtYY"
#define SDKKEY    "ERmWGed5GcWtsE9WGG22u9d2JTpaqQewRd6vzHyijJmx"

#define INPUT_IMAGE_FORMAT ASVL_PAF_RGB24_B8G8R8
#define WORKBUF_SIZE (40*1024*1024)
#define MAX_FACE_NUM (50)

FaceDetectHong::FaceDetectHong(int w, int h):FaceDetect(w,h)
{
    p_workmem = nullptr;

    // face detect sdk init
    p_workmem = (MByte *)malloc(WORKBUF_SIZE);
    if(p_workmem == nullptr)
    {
        fprintf(stderr, "fail to malloc workbuf\r\n");
        exit(0);
    }

    h_engine = nullptr;
    int ret = AFD_FSDK_InitialFaceEngine(APPID, SDKKEY, p_workmem, WORKBUF_SIZE,
                                        &h_engine, AFD_FSDK_OPF_0_HIGHER_EXT, 16, MAX_FACE_NUM);
    if(ret != 0)
    {
        fprintf(stderr, "fail to AFD_FSDK_InitialFaceEngine(): 0x%x\r\n", ret);
        free(p_workmem);
        exit(0);
    }
    
    // image buf init
    input_img = {0};

    image_buf_len = image_width * image_height * sizeof(uint8_t) * 3;
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

}

FaceDetectHong::~FaceDetectHong()
{
    free(input_img.ppu8Plane[0]);
    
    AFD_FSDK_UninitialFaceEngine(h_engine);
    if(p_workmem != nullptr)
    {
        free(p_workmem);
    }
}

bool FaceDetectHong::detectFace(const cv::Mat &im)
{
    memcpy(input_img.ppu8Plane[0], im.data, image_buf_len);

    LPAFD_FSDK_FACERES face_result;
    int ret = AFD_FSDK_StillImageFaceDetection(h_engine, &input_img, &face_result);
    if(ret != 0)
    {
        fprintf(stderr, "fail to AFD_FSDK_StillImageFaceDetection(): 0x%x\r\n", ret);
        return false;
    }

    for(int i = 0; i < face_result->nFace; ++i)
    {
        cv::Rect rect(face_result->rcFace[i].left, face_result->rcFace[i].top,
            face_result->rcFace[i].right - face_result->rcFace[i].left,
            face_result->rcFace[i].bottom - face_result->rcFace[i].top);
        face_roi.push_back(rect);
    }

    return true;
}