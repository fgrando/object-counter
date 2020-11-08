#include "Mask.h"

using namespace std;
using namespace cv;

void Mask::draw(cv::Mat& frame){
    
    m_mask = frame.clone();
    
    for(auto& poly : m_polygons){
        
        const Point *pts = (const Point*) Mat(poly).data;
        int npts = Mat(poly).rows;
        polylines(m_mask, &pts, &npts, 1, true, Scalar(0, 255, 0));

        if (poly.size() > 2){
            Scalar color(0, 255, 0);
            fillPoly(m_mask, &pts, &npts, 1, color, LINE_8);
        }
    }

    imshow("mask", m_mask);
}

void Mask::addPoly(const std::vector<cv::Point>& poly){
    
    if (poly.size() > 2){
        m_polygons.push_back(poly);
    }
}