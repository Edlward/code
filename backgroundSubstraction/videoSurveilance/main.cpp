#include <opencv2/opencv.hpp>
// #include "countBlobTrack.h"
#include <opencv2/legacy/blobtrack.hpp>
#include <opencv2/legacy/legacy.hpp>

#include "fgvibe.h"
#include "blobDetectorPeople.h"
#include "blobTrackDraw.h"
using namespace cv;

/* Special extended blob structure for auto blob tracking: */
typedef struct CvBlobTrackAuto
{
    CvBlob  blob;
    int     BadFrames;
} CvBlobTrackAuto;


int main(int argc, char **argv)
{
    VideoCapture cap;
    cap.open("/home/lxg/codedata/walmat.mp4");
    // cap.open("/home/lxg/codedata/track.avi");
    
    if(!cap.isOpened())
    {
        printf("can not open camera\n");
        return -1;
    }

    // CvBlobTrackerAutoParam1 param;
    // param.FGTrainFrames = 2;
    // CvBlobTrackerAuto *track =  cvCreateBlobTrackerAuto1(&param);
    
    // CvFGDetector *fg = cvCreateFGDetectorBase(CV_BG_MODEL_FGD, NULL);
    // CvFGDetector *fg = cvCreateFGDetectorBase(CV_BG_MODEL_MOG, NULL);
    // CvFGDetector *fg = cvCreateFGDetectorBase(CV_BG_MODEL_FGD_SIMPLE, NULL);
    CvFGDetector *m_fg = new Fgvibe;

    // CvBlobTracker *m_bt = cvCreateBlobTrackerMS();
    // CvBlobTracker *m_bt = cvCreateBlobTrackerList(); // has parameter
    CvBlobTracker *m_bt = cvCreateBlobTrackerCC();

    // CvBlobDetector *m_bd = cvCreateBlobDetectorSimple();
    // CvBlobDetector *m_bd = cvCreateBlobDetectorCC();
    CvBlobDetector *m_bd = new BlobDetectorPeople;
    
    // CvBlobTrackGen *m_btg = cvCreateModuleBlobTrackGenYML();
    // CvBlobTrackGen *m_btg = cvCreateModuleBlobTrackGen1();
    // BlobTrackDraw *m_btg = new BlobTrackDraw;
    

    Mat im;
    char key = 0;
    
    CvBlobSeq blob_list;
    blob_list.AddFormat("i");

    int next_blob_id = 0;
    int frame_num = 0;

    while(key != 27)
    {
        printf("frameNum:%d ", frame_num);
        ++frame_num;

        double t = (double)getTickCount();

        cap >> im;
        if(im.empty())
        {
            printf("video end\n");
            break;
        }
        // cvtColor(im, im, CV_RGB2GRAY);

        IplImage im_src;
        im_src = IplImage(im);

        // foreground get
        IplImage *pfg_image = NULL;
        {
            // CvSize s = cvSize(pfg_image->width, pfg_image->height);
            // IplImage *pgray = cvCreateImage(s, 8, 1);
            // cvCvtColor(im_src, pgray, CV_RGB2GRAY);

            m_fg->Process(&im_src);
            pfg_image = m_fg->GetMask();
            if(pfg_image == NULL)
            {
                printf("empty mask\n");
            }
            cvNamedWindow("mask");
            cvShowImage("mask", pfg_image);
        }

        // process pfg_image 
        if(0)
        {
            CvSize s = cvSize(pfg_image->width, pfg_image->height);
            IplImage *pfg_tmp = cvCreateImage(s, pfg_image->depth, pfg_image->nChannels);
            // IplConvKernel *conv = cvCreateStructuringElementEx(5, 5, 3, 3, MORPH_RECT);
            // for(int i = 0; i < 2; ++i)
            {
                cvDilate(pfg_image, pfg_tmp, 0, 1);
                cvErode(pfg_tmp, pfg_image, 0, 1);
            }
            
            cvShowImage("dilate", pfg_image);
            cvReleaseImage(&pfg_tmp);
            // cvReleaseStructuringElement(&conv);
        }

        // track blob
        if(1)
        {
            double t = (double)getTickCount();

            m_bt->Process(&im_src, pfg_image);
            for(int i = blob_list.GetBlobNum(); i > 0; --i)
            {
                // update data of tracked blob list
                CvBlob *pb = blob_list.GetBlob(i-1);
                int blob_id = CV_BLOB_ID(pb);
                int idx = m_bt->GetBlobIndexByID(blob_id);
                m_bt->ProcessBlob(idx, pb, &im_src, pfg_image);
                pb->ID = blob_id;
            }

            printf("track:%f ", ((double)getTickCount() - t) / getTickFrequency());
        }

        // blob deleter
        if(1)
        {
            int w = pfg_image->width;
            int h = pfg_image->height;

            int border = 10;

            for(int i = 0; i < blob_list.GetBlobNum(); ++i)
            {
                CvBlobTrackAuto *pb = (CvBlobTrackAuto*)blob_list.GetBlob(i);

                if(pb->blob.x < border || pb->blob.y < border || 
                    pb->blob.x + pb->blob.w/2 > w - border || pb->blob.y+pb->blob.h/2 > h - border)
                {
                    m_bt->DelBlobByID(CV_BLOB_ID(pb));
                    blob_list.DelBlob(i);
                    i--;
                    continue;
                }

                CvRect r = CV_BLOB_RECT(pb);
                CvMat mat;
                double aver = 0;
                double area = CV_BLOB_WX(pb) * CV_BLOB_WY(pb);

                if(r.x < 0){r.width += r.x;r.x = 0;}
                if(r.y < 0){r.height += r.y;r.y = 0;}
                if(r.x+r.width>=w){r.width = w-r.x-1;}
                if(r.y+r.height>=h){r.height = h-r.y-1;}
                if(r.width > 4 && r.height > 4 && r.x < w && r.y < h &&
                    r.x >=0 && r.y >=0 &&
                    r.x+r.width < w && r.y+r.height < h && area > 0)
                {
                    int sum = cvSum(cvGetSubRect(pfg_image, &mat, r)).val[0];
                    aver = 1.0f * sum / area;
                    /* if mask in blob area exists then its blob OK*/
                    // if(aver > 0.1*255) Good = 1;
                    if(aver < 25)
                    {
                        m_bt->DelBlobByID(CV_BLOB_ID(pb));
                        blob_list.DelBlob(i);
                        i--;    
                    }
                }
            }
        }   /*  Blob deleter. */

        // update blobs
        if(m_bt)
        {
            m_bt->Update(&im_src, pfg_image);
        }


        // detect new blob
        {
            static CvBlobSeq new_blob_list;
            CvBlobTrackAuto newb;
            new_blob_list.Clear();

            if(m_bd->DetectNewBlob(&im_src, pfg_image, &new_blob_list, &blob_list))
            {
                IplImage *pmask = pfg_image;

                for(int i = 0; i < new_blob_list.GetBlobNum(); ++i)
                {
                    CvBlob *pbn = new_blob_list.GetBlob(i);

                    if(pbn && pbn->w >= 5 && pbn->h >= 5)
                    {
                        pbn->ID = next_blob_id;
                        CvBlob *pb = m_bt->AddBlob(pbn, &im_src, pmask);
                        if(pb)
                        {
                            newb.blob = pb[0];
                            newb.BadFrames = 0;
                            blob_list.AddBlob((CvBlob*)&newb);
                            ++next_blob_id;
                        }
                    }
                }
                if(pmask != pfg_image) cvReleaseImage(&pmask);
            }
            printf("detectBlob:%d ", new_blob_list.GetBlobNum());

            // show detected new blob
            if(1)
            {
                IplImage *imtmp = cvCloneImage(&im_src);

                for(int i = new_blob_list.GetBlobNum(); i > 0; --i)
                {
                    CvBlob *pb = new_blob_list.GetBlob(i-1);
                    CvRect rect = CV_BLOB_RECT(pb);
                    cvRectangleR(imtmp, rect, CV_RGB(0,255,0), 1, CV_AA, 0);
                    cvShowImage("showNewBlob", imtmp);
                    // waitKey(0);
                }
                cvReleaseImage(&imtmp);
            }
        }


        // debug track
        if(1)
        {
            CvFont font;
            int line_type = CV_AA;
            IplImage *pi = cvCloneImage(&im_src);
            char str[1024];

            cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, 0, 1, line_type);
            for(int i = blob_list.GetBlobNum(); i > 0; --i)
            {
                CvSize text_size;
                CvBlob *pb = blob_list.GetBlob(i-1);
                CvPoint p = cvPoint(cvRound(pb->x*256), cvRound(pb->y*256));// 这里与cvEllipse有关系，有位移运算
                CvSize s = cvSize(MAX(1, cvRound(CV_BLOB_RX(pb)*256)), MAX(1, cvRound(CV_BLOB_RY(pb)*256)));
                // int c = cvRound(255*) //getstate
                // int c = 0;

                // cvEllipse(pi, p, s, 0, 0, 360, CV_RGB(c,255-c,0), cvRound(1), CV_AA, 8);
                CvRect rect = CV_BLOB_RECT(pb);
                cvRectangleR(pi, rect, CV_RGB(0,255,0), 1, CV_AA, 0);

                CvRect r = CV_BLOB_RECT(pb);
                CvMat mat;
                double aver = 0;
                double area = CV_BLOB_WX(pb) * CV_BLOB_WY(pb);
                int w = pfg_image->width;
                int h = pfg_image->height;
                if(r.x < 0){r.width += r.x;r.x = 0;}
                if(r.y < 0){r.height += r.y;r.y = 0;}
                if(r.x+r.width>=w){r.width = w-r.x-1;}
                if(r.y+r.height>=h){r.height = h-r.y-1;}
                
                int sum = 0;
                if(r.width > 4 && r.height > 4 && r.x < w && r.y < h &&
                    r.x >=0 && r.y >=0 && r.x+r.width < w && r.y+r.height < h && area > 0)
                {
                    sum = cvSum(cvGetSubRect(pfg_image, &mat, r)).val[0];
                }
                CvBlobTrackAuto *pbtmp = (CvBlobTrackAuto *)blob_list.GetBlob(i-1);

                p.x >>= 8;
                p.y >>= 8;
                s.width >>= 8;
                s.height >>= 8;
                sprintf(str,"%03d,%3.f,%3.f",CV_BLOB_ID(pb), pb->w, pb->h);
                // sprintf(str,"%03d,%3.f,%3.f,%d",CV_BLOB_ID(pb), pb->w, pb->h, pbtmp->BadFrames);
                
                cvGetTextSize( str, &font, &text_size, NULL );
                p.y -= s.height;
                cvPutText( pi, str, p, &font, CV_RGB(0,255,255));
            }
            cvShowImage("debug", pi);
            cvReleaseImage(&pi);
        }

        // blob track generator, this do nothing help
        // if(m_btg)
        // {
        //     for(int i = blob_list.GetBlobNum(); i > 0; --i)
        //     {
        //         CvBlob *pb = blob_list.GetBlob(i-1);
        //         m_btg->AddBlob(pb);
        //     }
        //     m_btg->Process(&im_src, pfg_image);

        //     IplImage *btg_src = cvCloneImage(&im_src);
        //     m_btg->drawTrack(btg_src);
        //     cvShowImage("blobtrack", btg_src);
        //     cvWaitKey(1);
        //     CvRealseImage(btg_src);
        // }

        printf("blobNum:%d  ", blob_list.GetBlobNum());

        t = ((double)getTickCount() - t) / getTickFrequency();
        // printf("%d\n", im_src.width);
        printf("time: %f\n", t);

        // imshow("src", im);
        // cvShowImage("track", &im_src);

        key = waitKey(1);
        if(key == 's')
        {
            waitKey(0);
        }
    }
}


