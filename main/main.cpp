#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

#include "Control.h"

Control GL_Control;

void mouseInput(int event, int x, int y, int flags, void* data){
    GL_Control.readMouse(event, x, y);
}


int main(int argc, char *argv[])
{
    Mat frame;
    namedWindow(GL_Control.getWindowName(), WINDOW_AUTOSIZE);
    setMouseCallback(GL_Control.getWindowName(), mouseInput, NULL); // register after the window

    bool usePicture = false;

    if (usePicture) {
        frame = imread(GL_Control.getPictureSource());  
        
        GL_Control.updateFrame(frame);
        imshow(GL_Control.getWindowName(), frame);
        GL_Control.readKeyboard(waitKey(0 /*forever*/));
    
    } else {
        VideoCapture cap(-1);
        
        if (!cap.isOpened()) {
            cout << "FAILED to Open source '" << GL_Control.getVideoSource() << "'";
            exit(-1);
        }

        for(;;){
            bool result = cap.read(frame);
            
            GL_Control.updateFrame(frame);    
            imshow(GL_Control.getWindowName(), frame);
            GL_Control.readKeyboard(waitKey(1));
        }
    }
    
    return 0;
}
