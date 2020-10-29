#include "DetectorFSM.h"

#include <iostream>
using namespace std;

#include <opencv2/highgui/highgui.hpp>

using namespace Detector;

DetectorFSM::DetectorFSM()
{
    persist = false;
	// All lights are initially turned off
	currentState = &Idle::getInstance();
}

void DetectorFSM::setState(InputState& newState)
{
	currentState->exit(this);  // do stuff before we change state
	currentState = &newState;  // actually change states now
	currentState->enter(this); // do stuff after we change state
}

void DetectorFSM::process(int event, int x, int y, int kbdr)
{
    m_event = event;
    m_x = x;
    m_y = y;
    m_kbdr = kbdr;

	currentState->toggle(this);
}


DetectorFSM& DetectorFSM::getInstance(){
    static DetectorFSM s;
    return s;
}



void DetectorFSM::Draw(cv::Mat& frame){
        
    for(int i=0; i <  m_points.size(); i++){
        cout << m_points[i] << " ";

        if (i == (m_points.size() - 1))
            cout << endl;
    }


    if (m_points.size() > 1)
    {
        cv::Point p1 = m_points[0];
        cv::Point p2 = m_points[1];

        cv::Scalar colorLine(0,255,0); // Green
        int thicknessLine = 2;
        cv::line(frame, p1, p2, colorLine, thicknessLine);
    }
}


void DetectorFSM::Persist(vector<Line>& list){
    if (persist){
        Line line;
        line.a = m_points[0];
        line.b = m_points[1];
        list.push_back(line);
        persist = false;
    }
}




void Idle::enter(DetectorFSM* fsm){
    cout << "\tEntered Idle state" << endl;
    fsm->m_points.clear();
}

void Idle::toggle(DetectorFSM* fsm){
    if ((char)fsm->m_kbdr == 'l'){
        fsm->setState(MouseClicked::getInstance());
    }
}

Idle& Idle::getInstance(){
    static Idle s;
    return s;
}

void Idle::exit(DetectorFSM* fsm){
    cout << "\tleaving idle state" << endl;
}





void MouseClicked::toggle(DetectorFSM* fsm){

    if (//(m_lastEvent == cv::EVENT_MOUSEMOVE) &&
        (fsm->m_event == cv::EVENT_LBUTTONDOWN)){
        
        fsm->setState(DrawPreview::getInstance());

    }

    m_lastEvent = fsm->m_event;
}

MouseClicked& MouseClicked::getInstance(){
    static MouseClicked s;
    return s;
}
void MouseClicked::enter(DetectorFSM* fsm){
    cout << "waiting user click to begin" << endl;
}

void MouseClicked::exit(DetectorFSM* fsm){
    cout << "\t first point set" << endl;
    cout << "\t" << fsm->m_x << " " 
         << fsm->m_y << " " 
         << fsm->m_event << " " 
         << fsm->m_kbdr 
         << " len " << fsm->m_points.size() 
         << endl;
    
    fsm->m_points.push_back(cv::Point(fsm->m_x, fsm->m_y));
}





DrawPreview& DrawPreview::getInstance(){
    static DrawPreview s;
    return s;
}

void DrawPreview::enter(DetectorFSM* fsm){
    cout << "Adding preview point" << endl;
    fsm->m_points.push_back(cv::Point(fsm->m_x, fsm->m_y));
}

void DrawPreview::toggle(DetectorFSM* fsm){
    
    fsm->m_points.pop_back();
    fsm->m_points.push_back(cv::Point(fsm->m_x, fsm->m_y));

    if (fsm->m_event == cv::EVENT_LBUTTONUP){

        cout << "end point set " << fsm->m_x << " " 
         << fsm->m_y << " " 
         << fsm->m_event << " " 
         << fsm->m_kbdr 
         << " len " << fsm->m_points.size() 
         << endl;

        fsm->setState(Idle::getInstance());
    }
}

void DrawPreview::exit(DetectorFSM* fsm){
    cout << "\t persist line" << endl;
    fsm->persist = true;
}

