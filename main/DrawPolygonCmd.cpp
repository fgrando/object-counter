#include "DrawPolygonCmd.h"
#include <iostream>

using namespace std;
using namespace cv;

DrawPolygonCmd::DrawPolygonCmd(char key, cv::Scalar color)
    : m_state(State::Idle)
    , m_tempPoint(0,0)
    , m_key(key)
    , m_previousEvent(-1)
    , m_color(color)
{
    
}

DrawPolygonCmd::~DrawPolygonCmd(){}

std::string DrawPolygonCmd::getHelp(){
    return "d.....Draw Area (u for undo)";
}


void DrawPolygonCmd::process(const UserInput& input){

    bool lbuttonFallingEdge = (
        (m_previousEvent == EVENT_MOUSEMOVE) && (input.event == EVENT_LBUTTONDOWN));

    switch(m_state)
    {
    case State::Idle:
        if (input.kbdr == m_key){
            m_state = State::WaitOrigin;
            cout << "polycmd started" << endl;
        }
        break;
    
    case State::WaitOrigin:
        if (lbuttonFallingEdge){
            m_tempPoint = {input.x, input.y};
            m_points.push_back(m_tempPoint);

            cout << "origin set " << m_tempPoint << endl;
            m_state = State::Preview;
        }
        break;
    
    case State::Preview:
        m_tempPoint = {input.x, input.y};

        // undo
        if (input.kbdr == 'u'){
            if (m_points.size() > 1) {
                m_points.pop_back();
                m_tempPoint = m_points[m_points.size() -1];

                cout << m_points.size() << " points left" << endl;

            } else {
                m_points.clear();
                m_state = State::WaitOrigin;
                cout << "set origin" << endl;
            }

        } else {
            if (lbuttonFallingEdge){
                cout << "destination set " << m_tempPoint << endl;
                
                // save
                m_points.push_back(m_tempPoint);
                cout << m_points.size() << " points" << endl;
            }
        }

        if (input.event == cv::EVENT_RBUTTONDOWN){
            m_state = State::Idle;
            cout << "finished" << endl;

            for(auto& mask : m_masks){
                mask->addPoly(m_points);
            }

            m_points.clear();
        }

        break;
    }

    m_previousEvent = input.event;
}

void DrawPolygonCmd::draw(Mat& frame){
    cv::Mat overlay;
    const double alpha = 0.3;
    frame.copyTo(overlay);
    
    
    // draw the polygon
    vector<Point> contour = m_points;
    if (m_state == State::Preview){
        contour.push_back(m_tempPoint);
    }

    const Point *pts = (const Point*) Mat(contour).data;
    int npts = Mat(contour).rows;
    
    polylines(frame, &pts, &npts, 1, true, m_color);

    if (contour.size() > 2){
        fillPoly(overlay, &pts, &npts, 1, m_color, LINE_8);
    }

    // blend the overlay with the source image
    cv::addWeighted(overlay, alpha, frame, 1 - alpha, 0, frame);

}

void DrawPolygonCmd::addMask(Mask* mask){
    m_masks.push_back(mask);
}