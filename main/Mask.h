#ifndef E1EBC295_C5F1_4826_A3B3_8BB1D423481C
#define E1EBC295_C5F1_4826_A3B3_8BB1D423481C

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>

class AbstractMask{
public:
    virtual ~AbstractMask() {}

    virtual void initialize(cv::Mat& src) = 0;

    virtual void addPoly(const std::vector<cv::Point>& poly) = 0;

    virtual const cv::Mat& get() = 0;

    virtual void draw(cv::Mat& frame, cv::Scalar color) = 0;
};


class Mask : public AbstractMask{
public:
    Mask();
    ~Mask() {}

    void initialize(cv::Mat& src);

    void addPoly(const std::vector<cv::Point>& poly);

    inline const cv::Mat& get() { return m_mask; }

    virtual void draw(cv::Mat& frame, cv::Scalar color);

protected:
    void refresh();

    virtual inline cv::Scalar getBackground() = 0;
    virtual inline cv::Scalar getForeground() = 0;
    virtual inline std::string getWindowTitle() = 0;
    cv::Size m_srcSize;

    bool m_initialized;

    cv::Mat m_mask;

    // ares of interest
    std::vector< std::vector<cv::Point> > m_polygons;
};


#endif /* E1EBC295_C5F1_4826_A3B3_8BB1D423481C */
