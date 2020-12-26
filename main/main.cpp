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

#include "ImageCounter.h"

#include "BasicProcessing.h"










#include <iostream> // for standard I/O
#include <string>   // for strings
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat)
#include <opencv2/videoio.hpp>  // Video write
#include <chrono>
using namespace std;
using namespace cv;


int main2(int argc, char *argv[])
{
    int duration = 60;

    const bool askOutputType = true;  // If false it will use the inputs codec type

    VideoCapture inputVideo(DB::get().captureSource);
    if (!inputVideo.isOpened())
    {
        cout  << "Could not open the input video: " << DB::get().captureSource << endl;
        return -1;
    }

    const string NAME = "output.mp4";

    Size S = Size((int) inputVideo.get(CAP_PROP_FRAME_WIDTH),
                  (int) inputVideo.get(CAP_PROP_FRAME_HEIGHT));

    int sourceCodec = static_cast<int>(inputVideo.get(CAP_PROP_FOURCC));
    char SRC[] = {(char)(sourceCodec & 0XFF) , (char)((sourceCodec & 0XFF00) >> 8),(char)((sourceCodec & 0XFF0000) >> 16),(char)((sourceCodec & 0XFF000000) >> 24), 0};
    
    VideoWriter outputVideo;
    int outputCodec = VideoWriter::fourcc('H','2','6','4');
    outputVideo.open(NAME, outputCodec, inputVideo.get(CAP_PROP_FPS), S, true);
    if (!outputVideo.isOpened())
    {
        cout  << "Could not open the output video for write: " << NAME << endl;
        return -1;
    }
    
    int realCodec = static_cast<int>(outputVideo.get(CAP_PROP_FOURCC));
    char OUT[] = {(char)(realCodec & 0XFF) , (char)((realCodec & 0XFF00) >> 8),(char)((realCodec & 0XFF0000) >> 16),(char)((realCodec & 0XFF000000) >> 24), 0};


    cout << "Input FPS=" << inputVideo.get(CAP_PROP_FPS) << " Width=" << S.width << "  Height=" << S.height << " codec=" << SRC << endl;
    cout << "Output FPS=" << outputVideo.get(CAP_PROP_FPS) << " Width=" << S.width << "  Height=" << S.height << " codec=" << OUT << endl;

    Mat frame, final;


    auto start = std::chrono::system_clock::now();

    for(;;) //Show the image captured in the window and repeat
    {
        inputVideo >> frame;              // read
        if (frame.empty()) break;         // check if at end

        cv::flip(frame, final, -1);

        
        float scale = 0.5;
        cv::Scalar color(255,255,0);
        cv::Point origin(10, 20);
        cv::Size sz(200, 250);
 
        auto now = std::chrono::system_clock::now();
        std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
        putText(final, std::ctime(&timeNow), origin, FONT_HERSHEY_SIMPLEX, scale , color);
        outputVideo << final;

        imshow("out", final);

        std::chrono::duration<double> elapsed_seconds = now-start;
        if (elapsed_seconds.count() > duration+2)
            break;

    }
    return 0;
}




















#include "persistence.h"


int main(int argc, char *argv[])
{
    DB::get();
    GL_Control.init();

    Mat frame;
    namedWindow(GL_Control.getWindowName(), WINDOW_AUTOSIZE);
    setMouseCallback(GL_Control.getWindowName(), mouseInput, NULL); // register after the window

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
            exit(0);
        }

        Mat resized;
        cv::resize(frame, resized, cv::Size(), DB::get().captureResize, DB::get().captureResize);
        
        Mat final;
        cv::flip(resized, final, -1);

        BP::BasicProcessing::getInstance().process(final);
        GL_Control.updateFrame(final);

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
