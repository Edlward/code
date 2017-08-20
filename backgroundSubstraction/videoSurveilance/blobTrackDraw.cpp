#include "blobTrackDraw.h"

static void saveDrawTrack(DefBlobTrack* pTrack, char* pFileName, int norm = 0)
{   /* Save blob track: */
    int         j;
    FILE*       out = NULL;
    CvBlobSeq*  pS = pTrack->pSeq;
    CvBlob*     pB0 = pS?pS->GetBlob(0):NULL;

    if(pFileName == NULL) return;
    if(pTrack == NULL) return;

    out = fopen(pFileName,"at");
    if(out == NULL)
    {
        printf("Warning! Cannot open %s file for track output\n", pFileName);
        return;
    }

    fprintf(out,"%d",pTrack->FrameBegin);

    if(pS) for(j=0; j<pS->GetBlobNum(); ++j)
    {
        CvBlob* pB = pS->GetBlob(j);

        fprintf(out,", %.1f, %.1f", CV_BLOB_X(pB),CV_BLOB_Y(pB));

        if(CV_BLOB_WX(pB0)>0)
            fprintf(out,", %.2f",CV_BLOB_WX(pB)/(norm?CV_BLOB_WX(pB0):1));

        if(CV_BLOB_WY(pB0)>0)
            fprintf(out,", %.2f",CV_BLOB_WY(pB)/(norm?CV_BLOB_WY(pB0):1));
    }
    fprintf(out,"\n");
    fclose(out);
    pTrack->Saved = 1;
}   /* Save blob track. */

void BlobTrackDraw::Process(IplImage* /*pImg*/ , IplImage* /*pFG*/)
{
    int i;
    for(i=m_TrackList.GetBlobNum(); i>0; --i)
    {
        DefBlobTrack* pTrack = (DefBlobTrack*)m_TrackList.GetBlob(i-1);
        if(pTrack->FrameLast < m_Frame && !pTrack->Saved)
        {   /* Save track: */
            saveDrawTrack(pTrack, m_pFileName, m_BlobSizeNorm);
            if(pTrack->Saved)
            {   /* delete sequence */
                delete pTrack->pSeq;
                pTrack->pSeq = NULL;
                m_TrackList.DelBlob(i-1);
            }
        }   /* Save track. */
    }   /*  Check next track. */
    m_Frame++;
}

BlobTrackDraw::~BlobTrackDraw()
{
    int i;
    for(i=m_TrackList.GetBlobNum();i>0;--i)
    {
        DefBlobTrack* pTrack = (DefBlobTrack*)m_TrackList.GetBlob(i-1);
        if(!pTrack->Saved)
        {   /* Save track: */
            saveDrawTrack(pTrack, m_pFileName, m_BlobSizeNorm);
        }   /* Save track. */
        /* Delete sequence: */
        delete pTrack->pSeq;
        pTrack->pSeq = NULL;
    }   /* Check next track. */
}  

void BlobTrackDraw::AddBlob(CvBlob* pBlob)
{
    DefBlobTrack* pTrack = (DefBlobTrack*)m_TrackList.GetBlobByID(CV_BLOB_ID(pBlob));
    if(pTrack==NULL)
    {   /* Add new track: */
        DefBlobTrack    Track;
        Track.blob = pBlob[0];
        Track.FrameBegin = m_Frame;
        Track.pSeq = new CvBlobSeq;
        Track.Saved = 0;
        m_TrackList.AddBlob((CvBlob*)&Track);
        pTrack = (DefBlobTrack*)m_TrackList.GetBlobByID(CV_BLOB_ID(pBlob));
    }   /* Add new track. */
    assert(pTrack);
    pTrack->FrameLast = m_Frame;
    assert(pTrack->pSeq);
    pTrack->pSeq->AddBlob(pBlob);
}

void BlobTrackDraw::drawTrack(IplImage *src)
{
    int track_num = m_TrackList.GetBlobNum();
    CvBlob *blob;
    CvPoint pcenter;
    CvScalar scolor = CV_RGB(255, 0, 0);

    for(int i = 0; i < track_num; ++i)
    {
        DefBlobTrack *dbt = (DefBlobTrack *)m_TrackList.GetBlob(i);
        CvBlobSeq *bs = dbt->pSeq;
        int blob_num = bs->GetBlobNum();

        for(int j = 0; j < blob_num; ++j)
        {
            blob = bs->GetBlob(j);
            // printf("%f, %f\n", blob->x, blob->y);
            pcenter.x = blob->x;
            pcenter.y = blob->y;
            cvCircle(src, pcenter, 2, scolor); 
        }
    }
}