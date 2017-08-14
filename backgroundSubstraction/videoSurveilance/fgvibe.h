#ifndef _FGVIBE_H_
#define _FGVIBE_H_

#include <opencv2/opencv.hpp>
#include <opencv2/legacy/blobtrack.hpp>

// #include "libvibe++/ViBe.h"
// #include "libvibe++/distances/Manhattan.h"
// #include "libvibe++/system/types.h"

#include "libvibe++/ViBe.h"
#include "libvibe++/distances/Manhattan.h"
#include "libvibe++/system/types.h"

// using namespace ViBe;

class Fgvibe : public CvFGDetector
{
protected:
    IplImage *foreground;
    
    cv::Mat bw_frame;
    cv::Mat segmentation_map;
    
    
    /* Parameterization of ViBe. */
    typedef ViBe::ViBeSequential<3, ViBe::Manhattan<3> > ViBe;

    ViBe *vibe;

    bool first_frame;

    int height, width;

public:    
    Fgvibe()
    {
        first_frame = true;
    }

    ~Fgvibe()
    {

    }
    
    inline IplImage* GetMask()
    {
        return foreground ? foreground : NULL;
    };

    void Process(IplImage *pImg)
    {
        if(first_frame)
        {
            height = pImg->height;
            width = pImg->width;
            
            if(pImg->nChannels > 1)
            {
                printf("pImg is not gray height:%d width:%d\n", height, width);
            }

            bw_frame.create(height, width, CV_8UC1);
            segmentation_map.create(height, width, CV_8UC1);
            
            CvSize s = cvSize(width, height);
            foreground = cvCreateImage(s, pImg->depth, 1);

            vibe = NULL;

            first_frame = false;
            /* Instantiation of ViBe. */
            vibe = new ViBe(height, width, (uint8_t *)(pImg->imageData));
            return ;
        }

        /* Segmentation and update. */
        vibe->segmentation((uchar *)pImg->imageData, segmentation_map.data);
        vibe->update((uchar *)pImg->imageData, segmentation_map.data);

        /* Post-processing: 3x3 median filter. */
        medianBlur(segmentation_map, segmentation_map, 3);
        
        IplImage ptmp(segmentation_map);
        cvCopy(&ptmp, foreground);

    }

    void Release()
    {
        if(vibe)
        {
            delete vibe;
        }
    }
};

#endif