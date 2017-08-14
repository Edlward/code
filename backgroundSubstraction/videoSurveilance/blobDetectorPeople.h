#ifndef _BLOBDETECTORT_H_
#define _BLOBDETECTORT_H_

#include <opencv2/opencv.hpp>
#include <opencv2/legacy/blobtrack.hpp>
#include <vector>

#define EBD_FRAME_NUM 5

class BlobDetectorPeople: public CvBlobDetector
{
public:
    BlobDetectorPeople();
    ~BlobDetectorPeople();
    int DetectNewBlob(IplImage * pimg, IplImage *pfg, CvBlobSeq *p_newlist, CvBlobSeq *p_oldlist);
    void Release(){delete this;}

protected:
    std::vector<std::vector<cv::Point> > m_lastcontour;
    // IplImage*       m_pMaskBlobNew;
    // IplImage*       m_pMaskBlobExist;
    // /* Lists of connected components detected on previous frames: */
    // CvBlobSeq*      m_pBlobLists[EBD_FRAME_NUM];

};

#endif