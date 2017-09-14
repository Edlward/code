#if defined(__linux__) || defined(LINUX) || defined(__APPLE__) || defined(ANDROID) || (defined(_MSC_VER) && _MSC_VER>=1800)

#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

const string WindowName = "Face Detection example";

class CascadeDetectorAdapter: public DetectionBasedTracker::IDetector
{
    public:
        CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector):
            IDetector(),
            Detector(detector)
        {
            CV_Assert(detector);
        }

        void detect(const cv::Mat &Image, std::vector<cv::Rect> &objects)
        {
            Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
        }

        virtual ~CascadeDetectorAdapter()
        {}

    private:
        CascadeDetectorAdapter();
        cv::Ptr<cv::CascadeClassifier> Detector;
 };

int main(int , char** )
{
    namedWindow(WindowName);

    // VideoCapture VideoStream("/home/lxg/zqq2017090802.avi");
    // VideoCapture VideoStream("/home/lxg/livet2.avi");
    // VideoCapture VideoStream("/home/lxg/zqq2017090804.avi");
    VideoCapture VideoStream("/home/lxg/zqq2017090802.avi");

    if (!VideoStream.isOpened())
    {
        printf("Error: Cannot open video stream from camera\n");
        return 1;
    }

    // std::string cascadeFrontalfilename = "../../data/lbpcascades/lbpcascade_frontalface.xml";
    std::string cascadeFrontalfilename = "/home/lxg/codedata/headXml/goodClassifier/cascade_harr_1950_10000_24_24.xml";
    

    cv::Ptr<cv::CascadeClassifier> cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
    cv::Ptr<DetectionBasedTracker::IDetector> MainDetector = makePtr<CascadeDetectorAdapter>(cascade);
    if ( cascade->empty() )
    {
      printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
      return 2;
    }

    cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
    cv::Ptr<DetectionBasedTracker::IDetector> TrackingDetector = makePtr<CascadeDetectorAdapter>(cascade);
    if ( cascade->empty() )
    {
      printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
      return 2;
    }

    DetectionBasedTracker::Parameters params;
    DetectionBasedTracker Detector(MainDetector, TrackingDetector, params);

    if (!Detector.run())
    {
        printf("Error: Detector initialization failed\n");
        return 2;
    }

    Mat ReferenceFrame;
    Mat GrayFrame;
    vector<Rect> Faces;
    vector<DetectionBasedTracker::Object> ob_faces;

    char key = 0;
    do
    {
        double t = (double)getTickCount();
        
        VideoStream >> ReferenceFrame;
        if(ReferenceFrame.empty())
        {
            break;
        }
        cvtColor(ReferenceFrame, GrayFrame, COLOR_RGB2GRAY);
        Detector.process(GrayFrame);
        Detector.getObjects(Faces);

        for (size_t i = 0; i < Faces.size(); i++)
        {
            rectangle(ReferenceFrame, Faces[i], Scalar(0,255,0));
        }

        Detector.getObjects(ob_faces);
        for(size_t i = 0; i < ob_faces.size(); ++i)
        {

        }

        key = waitKey(30);
        if(key == 'b')
        {
            waitKey(0);
        }

        imshow(WindowName, ReferenceFrame);
        printf("%f s \n", (getTickCount() - t) / getTickFrequency());
        
    } while (key != 27);

    Detector.stop();

    return 0;
}

#else

#include <stdio.h>
int main()
{
    printf("This sample works for UNIX or ANDROID or Visual Studio 2013+ only\n");
    return 0;
}

#endif