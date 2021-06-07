#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

// optical flow seems to resist to headlights and slow tree moves.

double FACTOR = 0.7;
int MIN_THRESHOLD = 128; // Tune this value to filter sensitivity (bigger = low sensitivity)

int main()
{
    VideoCapture capture(samples::findFile("cap4.mp4"));
    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }


    Mat frame, resized, prvs;
    capture >> frame;
    cv::resize(frame, resized, cv::Size(), FACTOR, FACTOR);

    cvtColor(resized, prvs, COLOR_BGR2GRAY);

    while(true){
        Mat frame2, next;
        capture >> frame;
        cv::resize(frame, frame2, cv::Size(), FACTOR, FACTOR);

        if (frame2.empty())
            break;

        cvtColor(frame2, next, COLOR_BGR2GRAY);
        Mat flow(prvs.size(), CV_32FC2);
        calcOpticalFlowFarneback(prvs, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

        // visualization
        Mat flow_parts[2];
        split(flow, flow_parts);
        Mat magnitude, angle, magn_norm;
        cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
        normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);

        Mat clouds, mask, colormask, res;
        magn_norm.convertTo(clouds, CV_8U, 255.0);
        threshold(clouds, mask, 100, 255, THRESH_BINARY);

        addWeighted(next, 0.2, mask, 0.8, 0, res);
        imshow("result", res);

        int keyboard = waitKey(1);
        if (keyboard == 'q' || keyboard == 27)
            break;
        prvs = next;
    }
}

