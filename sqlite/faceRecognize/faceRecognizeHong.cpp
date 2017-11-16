#include "faceRecognizeHong.h"

#define APPID     "6EtR9RZUeC55yirVhnz95fVJbB3YLKGnjyuogDKMLtYY"
#define SDKKEY    "ERmWGed5GcWtsE9WGG22u9d9Ts5o4HUJL6c3kSjLRLMK"
#define WORKBUF_SIZE        (40*1024*1024)

FaceRecognizeHong::FaceRecognizeHong(std::string dbname, FaceDetect *detector):FaceRecognize(dbname)
{
    p_workmem = (MByte *)malloc(WORKBUF_SIZE);
    if(p_workmem == nullptr){
        fprintf(stderr, "fail to malloc engine work buffer\r\n");
        exit(0);
    }

    h_engine = nullptr;

    int ret = AFR_FSDK_InitialEngine(APPID, SDKKEY, p_workmem, WORKBUF_SIZE, &h_engine);
    if (ret != 0) {
        fprintf(stderr, "fail to AFR_FSDK_InitialEngine(): 0x%x\r\n", ret);
        free(p_workmem);
        exit(0);
    }

    this->detector = detector;
}

FaceRecognizeHong::~FaceRecognizeHong()
{
    //    free(faceModels2.pbFeature);
    AFR_FSDK_UninitialEngine(h_engine);
    free(p_workmem);
}

bool FaceRecognizeHong::searchDb(cv::Mat &im)
{
    for(size_t i = 0; i < detector->face_roi.size(); ++i)
    {
        int j = detector->image_buf_len;
    }
}