#include "blobDetectorPeople.h"
#include "globalSet.h"
#include <opencv2/opencv.hpp>

using namespace cv;

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


/* Constructor of BlobDetector: */
BlobDetectorPeople::BlobDetectorPeople()
{
    int i = 0;
    m_pMaskBlobNew = NULL;
    m_pMaskBlobExist = NULL;
    for(i=0;i<EBD_FRAME_NUM;++i)m_pBlobLists[i] = NULL;

    SetModuleName("Simple");
}

/* Destructor of BlobDetector: */
BlobDetectorPeople::~BlobDetectorPeople()
{
    int i;
    if(m_pMaskBlobExist) cvReleaseImage(&m_pMaskBlobExist);
    if(m_pMaskBlobNew) cvReleaseImage(&m_pMaskBlobNew);
    for(i=0; i<EBD_FRAME_NUM; ++i) delete m_pBlobLists[i];
}   /* cvReleaseBlobDetector */

/* cvDetectNewBlobs
 * return 1 and fill blob pNewBlob by blob parameters
 * if new blob is detected:
 */
int BlobDetectorPeople::DetectNewBlob(IplImage* /*pImg*/, IplImage* pFGMask, CvBlobSeq* pNewBlobList, CvBlobSeq* pOldBlobList)
{
    int         result = 0;
    CvSize      S = cvSize(pFGMask->width,pFGMask->height);
    if(m_pMaskBlobNew == NULL ) m_pMaskBlobNew = cvCreateImage(S,IPL_DEPTH_8U,1);
    if(m_pMaskBlobExist == NULL ) m_pMaskBlobExist = cvCreateImage(S,IPL_DEPTH_8U,1);

    /* Shift blob list: */
    {
        int     i;
        if(m_pBlobLists[0]) delete m_pBlobLists[0];
        for(i=1;i<EBD_FRAME_NUM;++i)m_pBlobLists[i-1]=m_pBlobLists[i];
        m_pBlobLists[EBD_FRAME_NUM-1] = new CvBlobSeq;
    }   /* Shift blob list. */

    /* Create exist blob mask: */
    cvCopy(pFGMask, m_pMaskBlobNew);

    /* Create contours and add new blobs to blob list: */
    {   /* Create blobs: */
        CvBlobSeq       Blobs;
        CvMemStorage*   storage = cvCreateMemStorage();

#if 1
        {   /* Glue contours: */
            cvFindBlobsByCCClasters(m_pMaskBlobNew, &Blobs, storage );
        }   /* Glue contours. */
#else
        {   /**/
            IplImage*       pIB = cvCloneImage(m_pMaskBlobNew);
            CvSeq*          cnts = NULL;
            CvSeq*          cnt = NULL;
            cvThreshold(pIB,pIB,128,255,CV_THRESH_BINARY);
            cvFindContours(pIB,storage, &cnts, sizeof(CvContour), CV_RETR_EXTERNAL);

            /* Process each contour: */
            for(cnt = cnts; cnt; cnt=cnt->h_next)
            {
                CvBlob  NewBlob;

                /* Image moments: */
                double      M00,X,Y,XX,YY;
                CvMoments   m;
                CvRect      r = ((CvContour*)cnt)->rect;
                CvMat       mat;

                if(r.height < S.height*0.02 || r.width < S.width*0.02) continue;

                cvMoments( cvGetSubRect(m_pMaskBlobNew,&mat,r), &m, 0 );
                M00 = cvGetSpatialMoment( &m, 0, 0 );

                if(M00 <= 0 ) continue;

                X  = cvGetSpatialMoment( &m, 1, 0 )/M00;
                Y  = cvGetSpatialMoment( &m, 0, 1 )/M00;

                XX = (cvGetSpatialMoment( &m, 2, 0 )/M00) - X*X;
                YY = (cvGetSpatialMoment( &m, 0, 2 )/M00) - Y*Y;

                NewBlob = cvBlob(r.x+(float)X,r.y+(float)Y,(float)(4*sqrt(XX)),(float)(4*sqrt(YY)));

                Blobs.AddBlob(&NewBlob);

            }   /* Next contour. */

            cvReleaseImage(&pIB);

        }   /* One contour - one blob. */
#endif

        {   /* Delete small and intersected blobs: */
            int i;
            for(i=Blobs.GetBlobNum(); i>0; i--)
            {
                CvBlob* pB = Blobs.GetBlob(i-1);

                if(pB->h < S.height*g_minimum_blob || pB->w < S.width*g_minimum_blob)
                {
                    Blobs.DelBlob(i-1);
                    continue;
                }
                if(pOldBlobList)
                {
                    int j;
                    for(j=pOldBlobList->GetBlobNum(); j>0; j--)
                    {
                        CvBlob* pBOld = pOldBlobList->GetBlob(j-1);
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

        {   /* Bubble-sort blobs by size: */
            int N = Blobs.GetBlobNum();
            int i,j;
            for(i=1; i<N; ++i)
            {
                for(j=i; j>0; --j)
                {
                    CvBlob  temp;
                    float   AreaP, AreaN;
                    CvBlob* pP = Blobs.GetBlob(j-1);
                    CvBlob* pN = Blobs.GetBlob(j);
                    AreaP = CV_BLOB_WX(pP)*CV_BLOB_WY(pP);
                    AreaN = CV_BLOB_WX(pN)*CV_BLOB_WY(pN);
                    if(AreaN < AreaP)break;
                    temp = pN[0];
                    pN[0] = pP[0];
                    pP[0] = temp;
                }
            }

            /* Copy only first 10 blobs: */
            for(i=0; i<MIN(N,10); ++i)
            {
                m_pBlobLists[EBD_FRAME_NUM-1]->AddBlob(Blobs.GetBlob(i));
            }

        }   /* Sort blobs by size. */

        cvReleaseMemStorage(&storage);

    }   /* Create blobs. */

    /* Analyze blob list to find best blob trajectory: */
    {
        int     Count = 0;
        int     pBLIndex[EBD_FRAME_NUM];
        int     pBL_BEST[EBD_FRAME_NUM];
        int     i;
        int     finish = 0;
        double  BestError = -1;
        int     Good = 1;

        for(i=0; i<EBD_FRAME_NUM; ++i)
        {
            pBLIndex[i] = 0;
            pBL_BEST[i] = 0;
        }

        /* Check configuration exist: */
        for(i=0; Good && (i<EBD_FRAME_NUM); ++i)
            if(m_pBlobLists[i] == NULL || m_pBlobLists[i]->GetBlobNum() == 0)
                Good = 0;

        if(Good)
        do{ /* For each configuration: */
            CvBlob* pBL[EBD_FRAME_NUM];
            int     good = 1;
            double  Error = 0;
            CvBlob* pBNew = m_pBlobLists[EBD_FRAME_NUM-1]->GetBlob(pBLIndex[EBD_FRAME_NUM-1]);

            for(i=0; i<EBD_FRAME_NUM; ++i)  pBL[i] = m_pBlobLists[i]->GetBlob(pBLIndex[i]);

            Count++;

            /* Check intersection last blob with existed: */
            if(good && pOldBlobList)
            {   /* Check intersection last blob with existed: */
                int     k;
                for(k=pOldBlobList->GetBlobNum(); k>0; --k)
                {
                    CvBlob* pBOld = pOldBlobList->GetBlob(k-1);
                    if((fabs(pBOld->x-pBNew->x) < (CV_BLOB_RX(pBOld)+CV_BLOB_RX(pBNew))) &&
                       (fabs(pBOld->y-pBNew->y) < (CV_BLOB_RY(pBOld)+CV_BLOB_RY(pBNew))))
                        good = 0;
                }
            }   /* Check intersection last blob with existed. */

            /* Check distance to image border: */
            if(good)
            {   /* Check distance to image border: */
                CvBlob*  pB = pBNew;
                float    dx = MIN(pB->x,S.width-pB->x)/CV_BLOB_RX(pB);
                float    dy = MIN(pB->y,S.height-pB->y)/CV_BLOB_RY(pB);

                if(dx < 1.1 || dy < 1.1) good = 0;
            }   /* Check distance to image border. */

            /* Check uniform motion: */
            if(good)
            {
                int     N = EBD_FRAME_NUM;
                float   sum[2] = {0,0};
                float   jsum[2] = {0,0};
                float   a[2],b[2]; /* estimated parameters of moving x(t) = a*t+b*/

                int j;
                for(j=0; j<N; ++j)
                {
                    float   x = pBL[j]->x;
                    float   y = pBL[j]->y;
                    sum[0] += x;
                    jsum[0] += j*x;
                    sum[1] += y;
                    jsum[1] += j*y;
                }

                a[0] = 6*((1-N)*sum[0]+2*jsum[0])/(N*(N*N-1));
                b[0] = -2*((1-2*N)*sum[0]+3*jsum[0])/(N*(N+1));
                a[1] = 6*((1-N)*sum[1]+2*jsum[1])/(N*(N*N-1));
                b[1] = -2*((1-2*N)*sum[1]+3*jsum[1])/(N*(N+1));

                for(j=0; j<N; ++j)
                {
                    Error +=
                        pow(a[0]*j+b[0]-pBL[j]->x,2)+
                        pow(a[1]*j+b[1]-pBL[j]->y,2);
                }

                Error = sqrt(Error/N);

                if( Error > S.width*0.01 ||
                    fabs(a[0])>S.width*0.1 ||
                    fabs(a[1])>S.height*0.1)
                    good = 0;

            }   /* Check configuration. */


            /* New best trajectory: */
            if(good && (BestError == -1 || BestError > Error))
            {
                for(i=0; i<EBD_FRAME_NUM; ++i)
                {
                    pBL_BEST[i] = pBLIndex[i];
                }
                BestError = Error;
            }   /* New best trajectory. */

            /* Set next configuration: */
            for(i=0; i<EBD_FRAME_NUM; ++i)
            {
                pBLIndex[i]++;
                if(pBLIndex[i] != m_pBlobLists[i]->GetBlobNum()) break;
                pBLIndex[i]=0;
            }   /* Next time shift. */

            if(i==EBD_FRAME_NUM)finish=1;

        } while(!finish);   /* Check next time configuration of connected components. */

        #if 0
        {/**/
            printf("BlobDetector configurations = %d [",Count);
            int i;
            for(i=0; i<EBD_FRAME_NUM; ++i)
            {
                printf("%d,",m_pBlobLists[i]?m_pBlobLists[i]->GetBlobNum():0);
            }
            printf("]\n");

        }
        #endif

        if(BestError != -1)
        {   /* Write new blob to output and delete from blob list: */
            CvBlob* pNewBlob = m_pBlobLists[EBD_FRAME_NUM-1]->GetBlob(pBL_BEST[EBD_FRAME_NUM-1]);
            pNewBlobList->AddBlob(pNewBlob);

            for(i=0; i<EBD_FRAME_NUM; ++i)
            {   /* Remove blob from each list: */
                m_pBlobLists[i]->DelBlob(pBL_BEST[i]);
            }   /* Remove blob from each list. */

            result = 1;

        }   /* Write new blob to output and delete from blob list. */
    }   /*  Analyze blob list to find best blob trajectory. */

    return result;

}   /* cvDetectNewBlob */


