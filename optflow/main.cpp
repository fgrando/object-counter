#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "VideoSrc.h"
#include "OpticalFlow.h"
#include "Scene.h"
#include "utils.h"

using namespace cv;
using namespace std;

// optical flow seems to resist to headlights and slow tree moves.



int main(int argc, char* argv[]) {
    // first module: capture frame from video
    VideoSrc &vid = VideoSrc::instance();

    // second module: get raw frame and generate a frame with blobs
    OpticalFlowBlobs &optFlow = OpticalFlowBlobs::instance();

    // third module: get frame with blobs and classify and draw
    Scene& scene = Scene::instance();

    // verbose is enabled
    bool verbose = true;

    // check if all arguments were received
    if (argc > 1){
        string src(argv[1]);
        if (!vid.setCapture(src)){
            //error in opening the video input
            cerr << "Unable to open file '" + src + "'!" << endl;
            return RETURN_BADFILE;
        }
    } else {
        cerr << "Usage: " << argv[0] << " [-nogui] <filename.mp4>" << endl;
        return RETURN_BADARGS;
    }

    // check if we run in verbose mode
    for(int i = 0; i < argc; i++){
        string arg(argv[i]);
        if(arg == "-nogui") {
            verbose=false;
        }
    }



    Mat frame; // raw frame from the capture
    if(vid.get(frame) == false){ // capture a fist frame to initialize opticalflow module
        cerr << "Could not get frame from caputure" << endl;
        return RETURN_BADFRAME;
    }

    optFlow.init(frame); // initialize the module

    // main loop
    while(true) {

        // get frame from capture
        if (vid.get(frame) == false) {
            break;
        }

        // identify movements in the frame with blobs
        Mat frameWithBlobs;
        optFlow.getBlobs(frame, frameWithBlobs);

        // classify detected blobs
        scene.updateBlobs(frameWithBlobs);
        scene.draw(frame);
        scene.maintain();

        // show windows in verbose mode
        if (verbose) {
            int keyboard = waitKey(1);
            if (keyboard == 'q' || keyboard == 27)
                break;

            //drawRects(frameWithBlobs, frame);
            drawPolys(frameWithBlobs, frame);
            //drawCircles(frameWithBlobs, frame);

            imshow("optflow", frameWithBlobs);
            imshow("detected", frame);
        }
    }
}
