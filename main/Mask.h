#ifndef E1EBC295_C5F1_4826_A3B3_8BB1D423481C
#define E1EBC295_C5F1_4826_A3B3_8BB1D423481C

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>

class Mask{
public:
    void draw(cv::Mat& frame);

    void addPoly(const std::vector<cv::Point>& poly);

private:
    cv::Mat m_mask;

    std::vector< std::vector<cv::Point> > m_polygons;
};


#endif /* E1EBC295_C5F1_4826_A3B3_8BB1D423481C */
