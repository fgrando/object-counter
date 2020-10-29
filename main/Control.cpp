#include "Control.h"
#include <iostream>


#include <opencv2/highgui/highgui.hpp>


using namespace cv;
using namespace std;

void Control::readMouse(int event, int x, int y){
    cout << x << " " << y << endl;

    if  ( event == EVENT_LBUTTONDOWN )
    {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if  ( event == EVENT_RBUTTONDOWN )
    {
        cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if  ( event == EVENT_MBUTTONDOWN )
    {
        cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if ( event == EVENT_MOUSEMOVE )
    {
        cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

    }
}

void Control::readKeyboard(int key){
    
    if (key < 0)
    {
        return;
    }
    
    cout << key << endl;

    if ((char)key == 'q'){
        exit(0);
    }
}

void Control::updateFrame(Mat& frame){


    // last step is to draw
    cv::Point p1(100,100), p2(200,100);
    cv::Scalar colorLine(0,255,0); // Green
    int thicknessLine = 2;
    
    for(int i = 0; i < 2; i++){
        cv::line(frame, p1, p2, colorLine, thicknessLine);
    }
}
