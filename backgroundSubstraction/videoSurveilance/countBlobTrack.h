#ifndef _BLOBTRACK_H_
#define _BLOBTRACK_H_

#include <opencv2/opencv.hpp>
#include <opencv2/legacy/blobtrack.hpp>

// using namespace cv;  带cv的不用命名空间
/* Special extended blob structure for auto blob tracking: */
typedef struct CvBlobTrackAuto
{
    CvBlob  blob;
    int     BadFrames;
} CvBlobTrackAuto;
class CvBlobTrackerAuto2: public CvBlobTrackerAuto
{
public:
    CvBlobTrackerAuto2(CvBlobTrackerAutoParam1* param);
    ~CvBlobTrackerAuto2();
    CvBlob* GetBlob(int index){return m_BlobList.GetBlob(index);};
    CvBlob* GetBlobByID(int ID){return m_BlobList.GetBlobByID(ID);};
    int     GetBlobNum(){return m_BlobList.GetBlobNum();};
    virtual IplImage* GetFGMask(){return m_pFGMask;};
    float   GetState(int BlobID){return m_pBTA?m_pBTA->GetState(BlobID):0;};
    const char*   GetStateDesc(int BlobID){return m_pBTA?m_pBTA->GetStateDesc(BlobID):NULL;};
    /* Return 0 if trajectory is normal;
       return >0 if trajectory abnormal. */
    void Process(IplImage* pImg, IplImage* pMask = NULL);
    void Release(){delete this;};
private:
    IplImage*               m_pFGMask;
    int                     m_FGTrainFrames;
    CvFGDetector*           m_pFG; /* Pointer to foreground mask detector module. */
    CvBlobTracker*          m_pBT; /* Pointer to Blob tracker module. */
    int                     m_BTDel;
    int                     m_BTReal;
    CvBlobDetector*         m_pBD; /* Pointer to Blob detector module. */
    int                     m_BDDel;
    CvBlobTrackGen*         m_pBTGen;
    CvBlobTrackPostProc*    m_pBTPostProc;
    int                     m_UsePPData;
    CvBlobTrackAnalysis*    m_pBTA; /* Blob trajectory analyser. */
    CvBlobSeq               m_BlobList;
    int                     m_FrameCount;
    int                     m_NextBlobID;
    const char*                   m_TimesFile;
public:
    virtual void SaveState(CvFileStorage* fs)
    {
        cvWriteInt(fs,"FrameCount",m_FrameCount);
        cvWriteInt(fs,"NextBlobID",m_NextBlobID);
        m_BlobList.Write(fs,"BlobList");
    };
    virtual void LoadState(CvFileStorage* fs, CvFileNode* node)
    {
        CvFileNode* BlobListNode = cvGetFileNodeByName(fs,node,"BlobList");
        m_FrameCount = cvReadIntByName(fs,node, "FrameCount", m_FrameCount);
        m_NextBlobID = cvReadIntByName(fs,node, "NextBlobID", m_NextBlobID);
        if(BlobListNode)
        {
            m_BlobList.Load(fs,BlobListNode);
        }
    };
};

CvBlobTrackerAuto* cvCreateBlobTrackerAuto2(CvBlobTrackerAutoParam1* param);

#endif