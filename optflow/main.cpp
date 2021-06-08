#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

#include "VideoSrc.h"
#include "OpticalFlow.h"
#include "Scene.h"

using namespace cv;
using namespace std;

// optical flow seems to resist to headlights and slow tree moves.

void drawRects(Mat& src, Mat& dst, Scalar color=Scalar(0, 0, 255));
void drawPolys(Mat& src, Mat& dst, Scalar color=Scalar(0, 0, 255));
void drawCircles(Mat& src, Mat& dst, Scalar color=Scalar(0, 0, 255));

int main() {
    VideoSrc &vid = VideoSrc::instance();

    if (!vid.setCapture("cap2.mp4")){
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }

    OpticalFlowBlobs &optFlow = OpticalFlowBlobs::instance();
    Mat frame, blobs;

    vid.get(frame);
    optFlow.init(frame);

    Scene& scene = Scene::instance();

    bool verbose = false;

    while(true){
        Mat next;
        vid.get(frame);
        if (frame.empty())
            break;

        optFlow.getBlobs(frame, blobs);
        if (verbose)
            imshow("optflow", blobs);


        int keyboard = waitKey(1);
        if (keyboard == 'q' || keyboard == 27)
            break;

        if (verbose) {
            drawRects(blobs, frame);
            drawPolys(blobs, frame);
            //drawCircles(blobs, frame);
        }

        scene.updateBlobs(blobs);
        scene.draw(frame);

        bool save = false;
        scene.maintain();

        if (verbose)
            imshow("detected", frame);
    }
}


void drawPolys(Mat& src, Mat& dst, Scalar color){
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = src.clone();
    cv::findContours(contourOutput, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point> > contours_poly(contours.size());

    for( size_t i = 0; i< contours.size(); i++ )
    {
        approxPolyDP(contours[i], contours_poly[i], 3, true );
        drawContours(dst, contours_poly, (int)i, color);
    }
}

void drawCircles(Mat& src, Mat& dst, Scalar color){
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = src.clone();
    cv::findContours(contourOutput, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point> > contours_poly(contours.size());

    vector<Point2f>centers(contours.size());
    vector<float>radius(contours.size());

    for( size_t i = 0; i< contours.size(); i++ )
    {
        approxPolyDP(contours[i], contours_poly[i], 3, true );
        minEnclosingCircle(contours_poly[i], centers[i], radius[i]);
        circle( dst, centers[i], (int)radius[i], color, 1 );
    }
}

void drawRects(Mat& src, Mat& dst, Scalar color){
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = src.clone();
    cv::findContours(contourOutput, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point> > contours_poly(contours.size());

    vector<Rect> boundRect(contours.size());

    for( size_t i = 0; i< contours.size(); i++ )
    {
        approxPolyDP(contours[i], contours_poly[i], 3, true );
        boundRect[i] = boundingRect( contours_poly[i] );
        rectangle( dst, boundRect[i].tl(), boundRect[i].br(), color, 1 );
    }
}