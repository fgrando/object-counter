#ifndef E237D3DF_318F_4F32_987A_909C21AB7ACF
#define E237D3DF_318F_4F32_987A_909C21AB7ACF

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

#include "COP.h"

class ImageCounter{
public:
    ImageCounter(cv::Point a, cv::Point b, std::string);
    virtual ~ImageCounter();

    void draw(cv::Mat& frame);

    cv::Point m_end;
    cv::Point m_start;
    int m_counter;
    std::string m_label;
    TrackVector crossedBlobs;
};


#endif /* E237D3DF_318F_4F32_987A_909C21AB7ACF */
