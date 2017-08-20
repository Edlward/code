#ifndef _BLOBTRACKGENER_H_
#define _BLOBTRACKGENER_H_

#include <opencv2/legacy/blobtrack.hpp>


typedef struct DefBlobTrack
{
    CvBlob      blob;
    CvBlobSeq*  pSeq;
    int         FrameBegin;
    int         FrameLast;
    int         Saved; /* flag */
} DefBlobTrack;

class BlobTrackDraw:public CvBlobTrackGen
{

public:
    BlobTrackDraw(int BlobSizeNorm = 0):m_TrackList(sizeof(DefBlobTrack))
    {
        m_BlobSizeNorm = BlobSizeNorm;
        m_Frame = 0;
        m_pFileName = NULL;

        SetModuleName("Gen1");
    };

    ~BlobTrackDraw();
 /*  Destructor. */

    void SetFileName(char* pFileName){m_pFileName = pFileName;};

    void AddBlob(CvBlob* pBlob);

    void Process(IplImage* /*pImg*/ = NULL, IplImage* /*pFG*/ = NULL);

    void Release()
    {
        delete this;
    }
    void drawTrack(IplImage *src);

protected:
    int         m_Frame;
    char*       m_pFileName;
    CvBlobSeq   m_TrackList;
    int         m_BlobSizeNorm;
};  /* class BlobTrackDraw */


#endif