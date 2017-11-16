#include "face.h"
#include <iostream>

#define APPID_D     "6EtR9RZUeC55yirVhnz95fVJbB3YLKGnjyuogDKMLtYY"
#define SDKKEY_D    "ERmWGed5GcWtsE9WGG22u9d2JTpaqQewRd6vzHyijJmx"

#define APPID_R     "6EtR9RZUeC55yirVhnz95fVJbB3YLKGnjyuogDKMLtYY"
#define SDKKEY_R    "ERmWGed5GcWtsE9WGG22u9d9Ts5o4HUJL6c3kSjLRLMK"

#define INPUT_IMAGE_FORMAT ASVL_PAF_RGB24_B8G8R8
#define WORKBUF_SIZE (40*1024*1024)
#define MAX_FACE_NUM (50)

Face::Face(std::string dbname, int w, int h)
{
    image_width = w;
    image_height = h;

    p_detect_workmem = nullptr;
    
    // face detect sdk init
    p_detect_workmem = (MByte *)malloc(WORKBUF_SIZE);
    if(p_detect_workmem == nullptr)
    {
        fprintf(stderr, "fail to malloc workbuf\r\n");
        exit(0);
    }

    h_detect_engine = nullptr;
    int ret;
    ret = AFD_FSDK_InitialFaceEngine(APPID_D, SDKKEY_D, p_detect_workmem, WORKBUF_SIZE,
                                        &h_detect_engine, AFD_FSDK_OPF_0_HIGHER_EXT, 16, MAX_FACE_NUM);
    if(ret != 0)
    {
        fprintf(stderr, "fail to AFD_FSDK_InitialFaceEngine(): 0x%x\r\n", ret);
        free(p_detect_workmem);
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
        AFD_FSDK_UninitialFaceEngine(h_detect_engine);
        free(p_detect_workmem);
        exit(0);
    }
    

    // recognize init
    p_recognize_workmem = (MByte *)malloc(WORKBUF_SIZE);
    if(p_recognize_workmem == nullptr){
        fprintf(stderr, "fail to malloc engine work buffer\r\n");
        exit(0);
    }

    h_recognize_engine = nullptr;

    ret = AFR_FSDK_InitialEngine(APPID_R, SDKKEY_R, p_recognize_workmem, WORKBUF_SIZE, &h_recognize_engine);
    if (ret != 0) {
        fprintf(stderr, "fail to AFR_FSDK_InitialEngine(): 0x%x\r\n", ret);
        free(p_recognize_workmem);
        exit(0);
    }

    // sqlite
    db = nullptr;
    if(sqlite3_open(dbname.c_str(), &db))
    {
        std::cout << "can not open sql database" << std::endl;
    }
}

Face::~Face()
{
    AFR_FSDK_UninitialEngine(h_recognize_engine);
    free(p_recognize_workmem);
    
    free(input_img.ppu8Plane[0]);
    
    AFD_FSDK_UninitialFaceEngine(h_detect_engine);
    if(p_detect_workmem != nullptr)
    {
        free(p_detect_workmem);
    }
}

bool Face::detectAndRecognize(cv::Mat &im)
{
    face_name.clear();
    memcpy(input_img.ppu8Plane[0], im.data, image_buf_len);
    
    // detect
    LPAFD_FSDK_FACERES face_detect;
    int ret = AFD_FSDK_StillImageFaceDetection(h_detect_engine, &input_img, &face_detect);
    if(ret != 0)
    {
        fprintf(stderr, "fail to AFD_FSDK_StillImageFaceDetection(): 0x%x\r\n", ret);
        return false;
    }

    for(int i = 0; i < face_detect->nFace; ++i)
    {
        cv::Rect rect(face_detect->rcFace[i].left, face_detect->rcFace[i].top,
            face_detect->rcFace[i].right - face_detect->rcFace[i].left,
            face_detect->rcFace[i].bottom - face_detect->rcFace[i].top);
    } 


    FaceBox facebox;

    // recognize
    AFR_FSDK_FACEINPUT face_recognize;
    for (int i = 0; i < face_detect->nFace; ++i)
    {
        face_recognize.lOrient = face_detect->lfaceOrient[i];
        face_recognize.rcFace.left = face_detect->rcFace[i].left;
        face_recognize.rcFace.top = face_detect->rcFace[i].top;
        face_recognize.rcFace.right = face_detect->rcFace[i].right;
        face_recognize.rcFace.bottom = face_detect->rcFace[i].bottom;

        facebox.box.x = face_detect->rcFace[i].left;
        facebox.box.y = face_detect->rcFace[i].top;
        facebox.box.width = face_detect->rcFace[i].right - face_detect->rcFace[i].left;
        facebox.box.height = face_detect->rcFace[i].bottom - face_detect->rcFace[i].top;
        
        
        AFR_FSDK_FACEMODEL face_model_1 = { 0 };
        printf("orient:%d, left:%d, top:%d, right:%d, bottom:%d\n", face_recognize.lOrient, 
                face_recognize.rcFace.left, face_recognize.rcFace.top, face_recognize.rcFace.right, face_recognize.rcFace.bottom);

        int ret = AFR_FSDK_ExtractFRFeature(h_recognize_engine, &input_img, &face_recognize, &face_model_1);
        if(ret != 0)
        {
            // MERR_INVALID_PARAM 2 参数输入非法
            // MERR_NO_MEMORY 4 内存不足
            // MERR_UNSUPPORTED 3
            fprintf(stderr, "fail to AFR_FSDK_ExtractFRFeature in Image A  ret:%d \r\n", ret);
            // AFR_FSDK_UninitialEngine(h_recognize_engine);
            // free(p_recognize_workmem);
            // exit(0);    

            facebox.similar = 0;
            facebox.name = "unknown";
            face_name.push_back(facebox);
            continue;
        }

        // sqlite prepare
        std::string sql = "select * from persons";
        ret = sqlite3_prepare(db, sql.c_str(), sql.length(), &stmt, NULL);
        if(ret != SQLITE_OK)
        {
            std::cout << sqlite3_errmsg(db);
            return false;
        }

        // search database
        AFR_FSDK_FACEMODEL face_model_2 = { 0 };
        const MByte *feature = nullptr;
        MFloat  fSimilScore = 0.0f;
        MFloat similar_max = 0.0f;

        std::string person_similar;
        while(true)
        {
            ret = sqlite3_step(stmt);
            if(ret == SQLITE_DONE)
            {
                break;
            }
            const char *person = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
            std::string person_name(const_cast<char *>(person));
            
            feature = reinterpret_cast<const MByte *>(sqlite3_column_blob(stmt, 3));
            face_model_2.lFeatureSize = face_model_1.lFeatureSize;
            face_model_2.pbFeature = const_cast<MByte *>(feature);

            // 由特征计算匹配度
            ret = AFR_FSDK_FacePairMatching(h_recognize_engine, &face_model_1, &face_model_2, &fSimilScore);
            if(similar_max < fSimilScore)
            {
                similar_max = fSimilScore;
                person_similar.swap(person_name);
            }
        }
        facebox.similar = similar_max;
        facebox.name.swap(person_similar);
        face_name.push_back(facebox);
    }
}

void Face::showFace(cv::Mat &im)
{
    cv::Mat im_show;
    im.copyTo(im_show);

    cv::Point2d pcorner;
    for(size_t i = 0; i < face_name.size(); ++i)
    {
        pcorner.x = face_name[i].box.x;
        pcorner.y = face_name[i].box.y;

        cv::putText(im_show, face_name[i].name, pcorner, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255), 1, 8);
        cv::rectangle(im_show, face_name[i].box, cv::Scalar(0,0,255), 2);
        std::cout << "name:" << face_name[i].name << "\tsimilarity:"<< face_name[i].similar << std::endl;
    }
    cv::imshow("face", im_show);
}