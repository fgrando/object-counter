#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

#include "Scene.h"
#include "persistence.h"

Scene GL_Control;

void mouseInput(int event, int x, int y, int flags, void* data){
    GL_Control.readMouse(event, x, y);
}

#include "BasicProcessing.h"


int main(int argc, char *argv[])
{
    DB::get();
    GL_Control.init();

    Mat frame;
    if (DB::get().mainVerbose) {
        namedWindow(GL_Control.getWindowName(), WINDOW_AUTOSIZE);
        setMouseCallback(GL_Control.getWindowName(), mouseInput, NULL); // register after the window
    }

    VideoCapture cap;
    if (argc > 1){
        cap=VideoCapture(argv[1]);
    } else {
        cap = VideoCapture(DB::get().captureSource);
    }


    if (!cap.isOpened()) {
        cout << "FAILED to Open source " << DB::get().captureSource << " '" << GL_Control.getVideoSource() << "'";
        exit(-1);
    }

    cap >> frame;

    // init

    BP::Parameters par; // add verbose
    par.bgSubtractor = DB::get().processSubtractor;
    par.blur = DB::get().captureBlur;
    par.erosion = DB::get().processErosion;
    par.erosionSize = DB::get().processErosionSize;
    par.dilation = DB::get().processDilation;
    par.dilationSize = DB::get().processDilationSize;
    par.threshold = DB::get().processThreshold;
    par.bgSubVerbose = DB::get().processBgSubVerbose;
    par.blurVerbose = DB::get().processBlurVerbose;
    par.erosionVerbose = DB::get().processErosionVerbose;
    par.dilationVerbose = DB::get().processDilationVerbose;
    par.thresholdVerbose = DB::get().processThresholdVerbose;

    BP::BasicProcessing::getInstance().initialize(par);

    long counter = 0;
    for(;;){
        static int interval = 1;
        static bool readFrame = true;

        if (readFrame) {
            cap >> frame;
        }
        //if (counter++ < 1) //625
        //    continue;

        if (frame.empty()) {
            GL_Control.finish();
            exit(0);
        }

        Mat resized;
        cv::resize(frame, resized, cv::Size(), DB::get().captureResize, DB::get().captureResize);

        Mat final;
        cv::flip(resized, final, -1);

        BP::BasicProcessing::getInstance().process(final);
        GL_Control.updateFrame(final);

        if (DB::get().mainVerbose)
            imshow(GL_Control.getWindowName(), final);

        int key = waitKey(interval);

        if (key == 'p') {
            readFrame = !readFrame;
            if (!readFrame) {
                cout << "PAUSE" << endl;
            } else {
                cout << "PLAY" << endl;
            }
        }
        GL_Control.readKeyboard(key);
    }
    return 0;
}
