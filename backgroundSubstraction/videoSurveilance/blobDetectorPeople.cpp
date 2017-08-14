#include "blobDetectorPeople.h"

#include <opencv2/opencv.hpp>

static int CompareContour(const void* a, const void* b, void* )
{
    float           dx,dy;
    float           h,w,ht,wt;
    CvPoint2D32f    pa,pb;
    CvRect          ra,rb;
    CvSeq*          pCA = *(CvSeq**)a;
    CvSeq*          pCB = *(CvSeq**)b;
    ra = ((CvContour*)pCA)->rect;
    rb = ((CvContour*)pCB)->rect;
    pa.x = ra.x + ra.width*0.5f;
    pa.y = ra.y + ra.height*0.5f;
    pb.x = rb.x + rb.width*0.5f;
    pb.y = rb.y + rb.height*0.5f;
    w = (ra.width+rb.width)*0.5f;
    h = (ra.height+rb.height)*0.5f;

    dx = (float)(fabs(pa.x - pb.x)-w);
    dy = (float)(fabs(pa.y - pb.y)-h);

    //wt = MAX(ra.width,rb.width)*0.1f;
    wt = 0;
    ht = MAX(ra.height,rb.height)*0.3f;
    return (dx < wt && dy < ht);
}


static void cvFindBlobsByCCClasters(IplImage* pFG, CvBlobSeq* pBlobs, CvMemStorage* storage)
{   /* Create contours: */
    IplImage*       pIB = NULL;
    CvSeq*          cnt = NULL;
    CvSeq*          cnt_list = cvCreateSeq(0,sizeof(CvSeq),sizeof(CvSeq*), storage );
    CvSeq*          clasters = NULL;
    int             claster_cur, claster_num;

    pIB = cvCloneImage(pFG);
    cvThreshold(pIB,pIB,128,255,CV_THRESH_BINARY);
    cvFindContours(pIB,storage, &cnt, sizeof(CvContour), CV_RETR_EXTERNAL);
    cvReleaseImage(&pIB);

    /* Create cnt_list.      */
    /* Process each contour: */
    for(; cnt; cnt=cnt->h_next)
    {
        cvSeqPush( cnt_list, &cnt);
    }

    claster_num = cvSeqPartition( cnt_list, storage, &clasters, CompareContour, NULL );

    for(claster_cur=0; claster_cur<claster_num; ++claster_cur)
    {
        int         cnt_cur;
        CvBlob      NewBlob;
        double      M00,X,Y,XX,YY; /* image moments */
        CvMoments   m;
        CvRect      rect_res = cvRect(-1,-1,-1,-1);
        CvMat       mat;

        for(cnt_cur=0; cnt_cur<clasters->total; ++cnt_cur)
        {
            CvRect  rect;
            CvSeq*  cont;
            int k = *(int*)cvGetSeqElem( clasters, cnt_cur );
            if(k!=claster_cur) continue;
            cont = *(CvSeq**)cvGetSeqElem( cnt_list, cnt_cur );
            rect = ((CvContour*)cont)->rect;

            if(rect_res.height<0)
            {
                rect_res = rect;
            }
            else
            {   /* Unite rects: */
                int x0,x1,y0,y1;
                x0 = MIN(rect_res.x,rect.x);
                y0 = MIN(rect_res.y,rect.y);
                x1 = MAX(rect_res.x+rect_res.width,rect.x+rect.width);
                y1 = MAX(rect_res.y+rect_res.height,rect.y+rect.height);
                rect_res.x = x0;
                rect_res.y = y0;
                rect_res.width = x1-x0;
                rect_res.height = y1-y0;
            }
        }

        if(rect_res.height < 1 || rect_res.width < 1)
        {
            X = 0;
            Y = 0;
            XX = 0;
            YY = 0;
        }
        else
        {
            cvMoments( cvGetSubRect(pFG,&mat,rect_res), &m, 0 );
            M00 = cvGetSpatialMoment( &m, 0, 0 );
            if(M00 <= 0 ) continue;
            X = cvGetSpatialMoment( &m, 1, 0 )/M00;
            Y = cvGetSpatialMoment( &m, 0, 1 )/M00;
            XX = (cvGetSpatialMoment( &m, 2, 0 )/M00) - X*X;
            YY = (cvGetSpatialMoment( &m, 0, 2 )/M00) - Y*Y;
        }
        NewBlob = cvBlob(rect_res.x+(float)X,rect_res.y+(float)Y,(float)(4*sqrt(XX)),(float)(4*sqrt(YY)));
        pBlobs->AddBlob(&NewBlob);

    }   /* Next cluster. */

    #if 0
    {   // Debug info:
        IplImage* pI = cvCreateImage(cvSize(pFG->width,pFG->height),IPL_DEPTH_8U,3);
        cvZero(pI);
        for(claster_cur=0; claster_cur<claster_num; ++claster_cur)
        {
            int         cnt_cur;
            CvScalar    color = CV_RGB(rand()%256,rand()%256,rand()%256);

            for(cnt_cur=0; cnt_cur<clasters->total; ++cnt_cur)
            {
                CvSeq*  cnt;
                int k = *(int*)cvGetSeqElem( clasters, cnt_cur );
                if(k!=claster_cur) continue;
                cnt = *(CvSeq**)cvGetSeqElem( cnt_list, cnt_cur );
                cvDrawContours( pI, cnt, color, color, 0, 1, 8);
            }

            CvBlob* pB = pBlobs->GetBlob(claster_cur);
            int x = cvRound(CV_BLOB_RX(pB)), y = cvRound(CV_BLOB_RY(pB));
            cvEllipse( pI,
                cvPointFrom32f(CV_BLOB_CENTER(pB)),
                cvSize(MAX(1,x), MAX(1,y)),
                0, 0, 360,
                color, 1 );
        }

        cvNamedWindow( "Clusters", 0);
        cvShowImage( "Clusters",pI );

        cvReleaseImage(&pI);

    }   /* Debug info. */
    #endif

}   /* cvFindBlobsByCCClasters */

BlobDetectorPeople::BlobDetectorPeople()
{
    int i = 0;
    m_pMaskBlobNew = NULL;
    m_pMaskBlobExist = NULL;
    for(i=0;i<EBD_FRAME_NUM;++i)m_pBlobLists[i] = NULL;

    SetModuleName("Simple");   
}
BlobDetectorPeople::~BlobDetectorPeople()
{
    int i;
    if(m_pMaskBlobExist) cvReleaseImage(&m_pMaskBlobExist);
    if(m_pMaskBlobNew) cvReleaseImage(&m_pMaskBlobNew);
    for(i=0; i<EBD_FRAME_NUM; ++i) delete m_pBlobLists[i];
}

int BlobDetectorPeople::DetectNewBlob(IplImage * pimg, IplImage *pfg, CvBlobSeq *p_newlist, CvBlobSeq *p_oldlist)
{
    int         result = 0;
    CvSize      S = cvSize(pfg->width, pfg->height);
    
    if(m_pMaskBlobNew == NULL ) 
    {
        m_pMaskBlobNew = cvCreateImage(S,IPL_DEPTH_8U,1);
    }
    if(m_pMaskBlobExist == NULL ) 
    {
        m_pMaskBlobExist = cvCreateImage(S,IPL_DEPTH_8U,1);
    }

    /* Shift blob list: */
    {
        int     i;
        if(m_pBlobLists[0]) 
        {
            delete m_pBlobLists[0];
        }
        for(i=1;i<EBD_FRAME_NUM;++i)
        {
            m_pBlobLists[i-1]=m_pBlobLists[i];
        }
        m_pBlobLists[EBD_FRAME_NUM-1] = new CvBlobSeq;
    }   /* Shift blob list. */
 
    /* Create exist blob mask: */
    cvCopy(pfg, m_pMaskBlobNew);

    /* Create contours and add new blobs to blob list: */
    {   /* Create blobs: */
        CvBlobSeq       Blobs;
        CvMemStorage*   storage = cvCreateMemStorage();

        {   /* Glue contours: */
            cvFindBlobsByCCClasters(m_pMaskBlobNew, &Blobs, storage );
        }   /* Glue contours. */
    
        
        {   /* Delete small and intersected blobs: */
            int i;
            for(i=Blobs.GetBlobNum(); i>0; i--)
            {
                CvBlob* pB = Blobs.GetBlob(i-1);
                if(pB->h < S.height*0.02 || pB->w < S.width*0.02)
                {
                    Blobs.DelBlob(i-1);
                    continue;
                }
                if(p_oldlist)
                {
                    int j;
                    for(j=p_oldlist->GetBlobNum(); j>0; j--)
                    {
                        CvBlob* pBOld = p_oldlist->GetBlob(j-1);
                        if((fabs(pBOld->x-pB->x) < (CV_BLOB_RX(pBOld)+CV_BLOB_RX(pB))) &&
                           (fabs(pBOld->y-pB->y) < (CV_BLOB_RY(pBOld)+CV_BLOB_RY(pB))))
                        {   /* Intersection is present, so delete blob from list: */
                            Blobs.DelBlob(i-1);
                            break;
                        }
                    }   /* Check next old blob. */
                }   /*  if pOldBlobList */
            }   /* Check next blob. */
        }   /*  Delete small and intersected blobs. */

        for(int i=0; i<Blobs.GetBlobNum(); ++i)
        {
            p_newlist->AddBlob(Blobs.GetBlob(i));
        }
        cvReleaseMemStorage(&storage);
    }

    // double  BestError = 0;

    // if(BestError != -1)
    // {   /* Write new blob to output and delete from blob list: */
    //     CvBlob* pNewBlob = m_pBlobLists[EBD_FRAME_NUM-1]->GetBlob(pBL_BEST[EBD_FRAME_NUM-1]);
    //     pNewBlobList->AddBlob(pNewBlob);
    //     for(i=0; i<EBD_FRAME_NUM; ++i)
    //     {   /* Remove blob from each list: */
    //         m_pBlobLists[i]->DelBlob(pBL_BEST[i]);
    //     }   /* Remove blob from each list. */
    //     result = 1;
    // }   /* Write new blob to output and delete from blob list. */

    // debug
    {
        IplImage *pi = cvCloneImage(pimg);

        for(int i = p_newlist->GetBlobNum(); i > 0; --i)
        {
            CvSize text_size;
            CvBlob *pb = p_newlist->GetBlob(i-1);
            CvRect rect = CV_BLOB_RECT(pb);
            cvRectangleR(pi, rect, CV_RGB(0,255,0), 1, CV_AA, 0);
        }
        cvShowImage("detectDebug", pi);
    }
}
