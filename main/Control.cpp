#include "Control.h"
#include <iostream>

using namespace Detector;

#include <opencv2/highgui/highgui.hpp>


using namespace cv;
using namespace std;

Control::Control() : m_input(nullptr) {}

void Control::readMouse(int event, int x, int y){
    //cout << x << " " << y << endl;

    //if  ( event == EVENT_LBUTTONDOWN )
    //{
    //    cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    //}
    //else if  ( event == EVENT_RBUTTONDOWN )
    //{
    //    cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    //}
    //else if  ( event == EVENT_MBUTTONDOWN )
    //{
    //    cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    //}
    //else if ( event == EVENT_MOUSEMOVE )
    //{
    //    cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
    //}

    m_mouseEvent = event;
    m_mouseX = x;
    m_mouseY = y;
}

void Control::readKeyboard(int key){
    m_kbdr = key;
    
    if ((char)key == 'q'){
        exit(0);
    }
}

void Control::updateFrame(Mat& frame){

    m_input = &DetectorFSM::getInstance();

    if (m_input != nullptr)
    {
        m_input->process(m_mouseEvent, m_mouseX, m_mouseY, m_kbdr);

        m_input->Draw(frame);

        m_input->Persist(m_lines);
    }

    for(int i=0; i <  m_lines.size(); i++){
        cv::Point& a = m_lines[i].a;
        cv::Point& b = m_lines[i].b;

        cv::Scalar colorLine(0,255,255);
        int thicknessLine = 2;
        cv::line(frame, a, b, colorLine, thicknessLine);
    }
}
