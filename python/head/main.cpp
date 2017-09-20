// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2016, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.

/*
Sample of using OpenCV dnn module with Tensorflow Inception model.
*/

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace cv::dnn;

#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;


void getMaxClass(const Mat &probBlob, int *classId, double *classProb);
std::vector<String> readClassNames(const char *filename);

int main(int argc, char **argv)
{

    String modelFile = "/home/lxg/code/python/tensorflow/headDetect/vggs.pb";
    String imageFile = "/home/lxg/codedata/headXml/headNeg/3.jpg";
    String inBlobName = "input";
    String outBlobName = "softmax";

    String classNamesFile = "/home/lxg/codedata/tensorflow/imagenet_comp_graph_label_strings.txt";

    //! [Create the importer of TensorFlow model]
    Ptr<dnn::Importer> importer;
    try                                     //Try to import TensorFlow AlexNet model
    {
        importer = dnn::createTensorflowImporter(modelFile);
    }
    catch (const cv::Exception &err)        //Importer can throw errors, we will catch them
    {
        std::cerr << err.msg << std::endl;
    }
    //! [Create the importer of Caffe model]
    if (!importer)
    {
        std::cerr << "Can't load network by using the mode file: " << std::endl;
        std::cerr << modelFile << std::endl;
        exit(-1);
    }

    printf("initialize net\n");
    //! [Initialize network]
    dnn::Net net;
    importer->populateNet(net);
    importer.release();                     //We don't need importer anymore
    //! [Initialize network]
    
    std::vector<String> p;
    p = net.getLayerNames();
    for(size_t i = 0; i < p.size(); ++i)
    {
        String s = p[i];
        printf("%d, name:%s\n", (int)i, s.c_str());
    }

    printf("read image\n");
    //! [Prepare blob]
    Mat img = imread(imageFile);
    if (img.empty())
    {
        std::cerr << "Can't read image from the file: " << imageFile << std::endl;
        exit(-1);
    }

    cv::Size inputImgSize = cv::Size(24, 24);

    if (inputImgSize != img.size())
        resize(img, img, inputImgSize);       //Resize image to input size
    printf("%d  %d  %d\n", img.cols, img.rows, img.channels());

    Mat inputBlob = blobFromImage(img);   //Convert Mat to image batchgoods_label_map.pbtxt
    //! [Prepare blob]
    // inputBlob -= 117.0;
    //! [Set input blob]
    printf("setinput %d  %d  %d\n", inputBlob.cols, inputBlob.rows, inputBlob.channels());
    net.setInput(inputBlob, inBlobName);        //set the network input
    //! [Set input blob]

    cv::TickMeter tm;
    tm.start();

    printf("tick\n");
    //! [Make forward pass]
    Mat result = net.forward(outBlobName); //compute output
    //! [Make forward pass]
    printf("stop\n");
    std::cout << result << endl;
    tm.stop();

    std::cout << "Output blob shape " << result.size[0] << " x " << result.size[1] << " x " << result.size[2] << " x " << result.size[3] << std::endl;
    std::cout << "Inference time, ms: " << tm.getTimeMilli()  << std::endl;

    return 0;
} //main
