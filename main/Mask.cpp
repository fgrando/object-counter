#include "Mask.h"

using namespace std;
using namespace cv;


Mask::Mask()
    : m_initialized(false)
{

}

void Mask::initialize(cv::Mat& src){

    if (!m_initialized) {
        m_srcSize = src.size();
        m_initialized = true;
        
        m_mask = Mat(m_srcSize, CV_8UC1, Scalar::all(255));
        //imshow(getWindowTitle(), m_mask);

        refresh();
    }
}

void Mask::refresh(){

    if (!m_initialized) {
        return;
    }

    m_mask = Mat(m_srcSize, CV_8UC1, getBackground());

    for(auto& poly : m_polygons){
        
        const Point *pts = (const Point*) Mat(poly).data;
        int npts = Mat(poly).rows;
        
        // wired poly
        //polylines(m_mask, &pts, &npts, 1, true, foreground);

        // this will fail if polygon has less than 2 points:
        fillPoly(m_mask, &pts, &npts, 1, getForeground(), LINE_8);
    }

    //imshow(getWindowTitle(), m_mask);

}

void Mask::addPoly(const std::vector<cv::Point>& poly){
    // less than 2 points in the polygon is not accepted
    if (poly.size() > 2){
        m_polygons.push_back(poly);

        refresh();
    }
}


void Mask::draw(cv::Mat& frame, cv::Scalar color) {
    cv::Mat overlay;
    double alpha = 0.3;

    // copy the source image to an overlay
    frame.copyTo(overlay);

    for(auto& poly : m_polygons){
        
        const Point *pts = (const Point*) Mat(poly).data;
        int npts = Mat(poly).rows;

        fillPoly(overlay, &pts, &npts, 1, color, LINE_8);
    }


    // blend the overlay with the source image
    cv::addWeighted(overlay, alpha, frame, 1 - alpha, 0, frame);
}

