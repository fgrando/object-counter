#include "ImageCounter.h"

#include <string>
#include <sstream>

using namespace std;
using namespace cv;

ImageCounter::~ImageCounter(){}

ImageCounter::ImageCounter(cv::Point a, cv::Point b, std::string label)
    : m_start(a)
    , m_end(b)
    , m_counter(0)
    , m_label(label)
{}

void ImageCounter::draw(cv::Mat& frame)
{
    Scalar color(0,0,255);
    int thickness = 1;

    line(frame, m_start, m_end, color, thickness);

    float scale = 0.4;

    stringstream ss;
    ss << m_label << "[" << m_counter << "]";
    string text = ss.str();

    putText(frame, text, m_end, FONT_HERSHEY_SIMPLEX, scale , color);
}
