#include "blobDetectorPeople.h"

#include <opencv2/opencv.hpp>

using namespace cv;

// BlobDetectorPeople::BlobDetectorPeople()
// {
//     // int i = 0;
//     // m_pMaskBlobNew = NULL;
//     // m_pMaskBlobExist = NULL;
//     // for(i=0;i<EBD_FRAME_NUM;++i)m_pBlobLists[i] = NULL;
// }

// BlobDetectorPeople::~BlobDetectorPeople()
// {
//     // int i;
//     // if(m_pMaskBlobExist) cvReleaseImage(&m_pMaskBlobExist);
//     // if(m_pMaskBlobNew) cvReleaseImage(&m_pMaskBlobNew);
//     // for(i=0; i<EBD_FRAME_NUM; ++i) delete m_pBlobLists[i];
// }

// int BlobDetectorPeople::DetectNewBlob(IplImage * pimg, IplImage *pfg, CvBlobSeq *p_newlist, CvBlobSeq *p_oldlist)
// {
//     IplImage *pfg_tmp;
//     pfg_tmp = cvCloneImage(pfg);
//     Mat src(pfg_tmp);
    
//     //Extract the contours so that
//     vector<vector<Point> > contours;
//     // vector<Vec4i> hierarchy;
//     // vector<vector<Point> > contours0;
//     // findContours( src, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//     findContours( src, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    
//     // delete contourArea small and translate to convexHull
//     vector<vector<Point> > convex_hulls;
//     vector<Point> convex;
//     int area_thresh = pimg->height * pimg->width / 500;

//     for(size_t i = 0; i < contours.size(); ++i)
//     {
//         if(contourArea(contours[i]) > area_thresh)
//         {
//             convexHull(contours[i], convex);
//             convex_hulls.push_back(convex);
//         }
//     }

//     // iterate through all the top-level contours,
//     // draw each connected component with its own random color
//     for(size_t idx = 0; idx < convex_hulls.size(); ++idx)
//     {
//         drawContours( src, convex_hulls, idx, Scalar(255,255,255), 0, 8 );
//     }

//     imshow("detectDebug", src);
// }
