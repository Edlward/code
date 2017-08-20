#ifndef _BLOBDETECTORT_H_
#define _BLOBDETECTORT_H_

#include <opencv2/opencv.hpp>
#include <opencv2/legacy/blobtrack.hpp>
#include <vector>

#define EBD_FRAME_NUM 3

// copy from opencv CvBlobDetectorSimple()
class BlobDetectorPeople: public CvBlobDetector
{
public:
    BlobDetectorPeople();
   ~BlobDetectorPeople();
    int DetectNewBlob(IplImage* pImg, IplImage* pFGMask, CvBlobSeq* pNewBlobList, CvBlobSeq* pOldBlobList);
    void Release(){delete this;}

protected:
    IplImage*       m_pMaskBlobNew;
    IplImage*       m_pMaskBlobExist;
    /* Lists of connected components detected on previous frames: */
    CvBlobSeq*      m_pBlobLists[EBD_FRAME_NUM];
};

#endif