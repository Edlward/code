#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "arcsoft_fsdk_face_recognition.h"
#include "merror.h"

//#define APPID     "your appid"
//#define SDKKEY    "your sdkkey"

#define INPUT1_IMAGE_FORMAT  ASVL_PAF_I420
//#define INPUT1_IMAGE_PATH    "your_input1_image.yuv"
#define INPUT1_IMAGE_WIDTH   (640)
#define INPUT1_IMAGE_HEIGHT  (480)

#define INPUT2_IMAGE_FORMAT  ASVL_PAF_I420
//#define INPUT2_IMAGE_PATH    "your_input2_image.yuv"
#define INPUT2_IMAGE_WIDTH   (640)
#define INPUT2_IMAGE_HEIGHT  (880)

#define WORKBUF_SIZE        (40*1024*1024)

int fu_ReadFile(const char* path, uint8_t **raw_data, size_t* pSize) {
    int res = 0;
    FILE *fp = 0;
    uint8_t *data_file = 0;
    size_t size = 0;

    fp = fopen(path, "rb");
    if (fp == nullptr) {
        res = -1;
        goto exit;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    data_file = (uint8_t *)malloc(sizeof(uint8_t)* size);
    if (data_file == nullptr) {
        res = -2;
        goto exit;
    }

    if (size != fread(data_file, sizeof(uint8_t), size, fp)) {
        res = -3;
        goto exit;
    }

    *raw_data = data_file;
    data_file = nullptr;
exit:
    if (fp != nullptr) {
        fclose(fp);
    }

    if (data_file != nullptr) {
        free(data_file);
    }

    if (nullptr != pSize) {
        *pSize = size;
    }
    
    return res;
}

int main(int argc, char* argv[]) {

    MByte *pWorkMem = (MByte *)malloc(WORKBUF_SIZE);
    if(pWorkMem == nullptr){
        fprintf(stderr, "fail to malloc engine work buffer\r\n");
        exit(0);
    }

    MHandle hEngine = nullptr;

    int ret = AFR_FSDK_InitialEngine(APPID, SDKKEY, pWorkMem, WORKBUF_SIZE, &hEngine);
    if (ret != 0) {
        fprintf(stderr, "fail to AFR_FSDK_InitialEngine(): 0x%x\r\n", ret);
        free(pWorkMem);
        exit(0);
    }

    const AFR_FSDK_Version*pVersionInfo = AFR_FSDK_GetVersion(hEngine);
    printf("%d %d %d %d\r\n", pVersionInfo->lCodebase, pVersionInfo->lMajor,
                                 pVersionInfo->lMinor, pVersionInfo->lBuild);
    printf("%s\r\n", pVersionInfo->Version);
    printf("%s\r\n", pVersionInfo->BuildDate);
    printf("%s\r\n", pVersionInfo->CopyRight);

    // image1
    ASVLOFFSCREEN inputImg1 = { 0 };
    inputImg1.u32PixelArrayFormat = INPUT1_IMAGE_FORMAT;
    inputImg1.i32Width = INPUT1_IMAGE_WIDTH;
    inputImg1.i32Height = INPUT1_IMAGE_HEIGHT;
    inputImg1.ppu8Plane[0] = nullptr;
    fu_ReadFile(INPUT1_IMAGE_PATH, (uint8_t**)&inputImg1.ppu8Plane[0], nullptr);
     if (!inputImg1.ppu8Plane[0]) {
        fprintf(stderr, "fail to fu_ReadFile(%s): %s\r\n", INPUT1_IMAGE_PATH, strerror(errno));
        AFR_FSDK_UninitialEngine(hEngine);
        free(pWorkMem);
        exit(0);
    }
    inputImg1.pi32Pitch[0] = inputImg1.i32Width;
    inputImg1.pi32Pitch[1] = inputImg1.i32Width/2;
    inputImg1.pi32Pitch[2] = inputImg1.i32Width/2;
    inputImg1.ppu8Plane[1] = inputImg1.ppu8Plane[0] + inputImg1.pi32Pitch[0] * inputImg1.i32Height;
    inputImg1.ppu8Plane[2] = inputImg1.ppu8Plane[1] + inputImg1.pi32Pitch[1] * inputImg1.i32Height/2;


    // image2
    ASVLOFFSCREEN inputImg2 = { 0 };
    inputImg2.u32PixelArrayFormat = INPUT2_IMAGE_FORMAT;
    inputImg2.i32Width = INPUT2_IMAGE_WIDTH;
    inputImg2.i32Height = INPUT2_IMAGE_HEIGHT;
    inputImg2.ppu8Plane[0] = nullptr;
    fu_ReadFile(INPUT2_IMAGE_PATH, (uint8_t**)&inputImg2.ppu8Plane[0], nullptr);
     if (!inputImg2.ppu8Plane[0]) {
        fprintf(stderr, "fail to fu_ReadFile(%s): %s\r\n", INPUT2_IMAGE_PATH, strerror(errno));
        free(inputImg1.ppu8Plane[0]);
        AFR_FSDK_UninitialEngine(hEngine);
        free(pWorkMem);
        exit(0);
    }
    inputImg2.pi32Pitch[0] = inputImg2.i32Width;
    inputImg2.pi32Pitch[1] = inputImg2.i32Width/2;
    inputImg2.pi32Pitch[2] = inputImg2.i32Width/2;
    inputImg2.ppu8Plane[1] = inputImg2.ppu8Plane[0] + inputImg2.pi32Pitch[0] * inputImg2.i32Height;
    inputImg2.ppu8Plane[2] = inputImg2.ppu8Plane[1] + inputImg2.pi32Pitch[1] * inputImg2.i32Height/2;

    // face1 提取人脸部特征
    AFR_FSDK_FACEMODEL faceModels1 = { 0 };
    {
        AFR_FSDK_FACEINPUT faceResult;
        faceResult.lOrient = AFR_FSDK_FOC_0;
        faceResult.rcFace.left = 282;
        faceResult.rcFace.top = 58;
        faceResult.rcFace.right = 422;
        faceResult.rcFace.bottom = 198;
        AFR_FSDK_FACEMODEL LocalFaceModels = { 0 };
        ret = AFR_FSDK_ExtractFRFeature(hEngine, &inputImg1, &faceResult, &LocalFaceModels);
        if(ret != 0){
            fprintf(stderr, "fail to AFR_FSDK_ExtractFRFeature in Image A\r\n");
            free(inputImg2.ppu8Plane[0]);
            free(inputImg1.ppu8Plane[0]);
            AFR_FSDK_UninitialEngine(hEngine);
            free(pWorkMem);
            exit(0);
        }
        faceModels1.lFeatureSize = LocalFaceModels.lFeatureSize;
        faceModels1.pbFeature = (MByte*)malloc(faceModels1.lFeatureSize);
        memcpy(faceModels1.pbFeature, LocalFaceModels.pbFeature, faceModels1.lFeatureSize);
        free(inputImg1.ppu8Plane[0]);
    }

    // face2 提取人脸部特征
    AFR_FSDK_FACEMODEL faceModels2 = { 0 };
    {
        AFR_FSDK_FACEINPUT faceResult;
        faceResult.lOrient = AFR_FSDK_FOC_0;
        faceResult.rcFace.left = 176;
        faceResult.rcFace.top = 57;
        faceResult.rcFace.right = 269;
        faceResult.rcFace.bottom = 151;
        AFR_FSDK_FACEMODEL LocalFaceModels = { 0 };
        ret = AFR_FSDK_ExtractFRFeature(hEngine, &inputImg2, &faceResult, &LocalFaceModels);
        if(ret != 0){
            fprintf(stderr, "fail to AFR_FSDK_ExtractFRFeature in Image B\r\n");
            free(inputImg2.ppu8Plane[0]);
            AFR_FSDK_UninitialEngine(hEngine);
            free(pWorkMem);
            exit(0);
        }
        faceModels2.lFeatureSize = LocalFaceModels.lFeatureSize;
        faceModels2.pbFeature = (MByte*)malloc(faceModels2.lFeatureSize);
        memcpy(faceModels2.pbFeature, LocalFaceModels.pbFeature, faceModels2.lFeatureSize);
        free(inputImg2.ppu8Plane[0]);
    }

    // 由特征计算匹配度
    MFloat  fSimilScore = 0.0f;
    ret = AFR_FSDK_FacePairMatching(hEngine, &faceModels1, &faceModels2, &fSimilScore);
    printf("fSimilScore ==  %f\r\n", fSimilScore);

    free(faceModels1.pbFeature);
    free(faceModels2.pbFeature);
    AFR_FSDK_UninitialEngine(hEngine);
    free(pWorkMem);

    return 0;
}
