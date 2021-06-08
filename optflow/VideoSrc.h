#ifndef ACE47ABA_7A37_4D21_AE85_BC8D9D861845
#define ACE47ABA_7A37_4D21_AE85_BC8D9D861845

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

using namespace cv;
class VideoSrc{
public:
    double ResizeFactor;

    VideoSrc() : ResizeFactor(0.7) {}

    inline bool setCapture(std::string filename) {
        m_name = filename;
        m_capture = VideoCapture(samples::findFile(filename));
        return m_capture.isOpened();
    }

    inline std::string name() { return m_name; }

    inline unsigned long long count() {
        unsigned long long frameidx = m_capture.get(CAP_PROP_POS_FRAMES);
        return frameidx;
    }

    inline unsigned long long total() {
        unsigned long long total = m_capture.get(CAP_PROP_FRAME_COUNT);
        return total;
    }


    inline void get(Mat& frame){
        Mat tmp;
        m_capture >> tmp;
        cv::resize(tmp, frame, cv::Size(), ResizeFactor, ResizeFactor);
    }

    inline static VideoSrc& instance() { static VideoSrc instance; return instance; }

private:
    VideoCapture m_capture;
    std::string m_name;
};

#endif /* ACE47ABA_7A37_4D21_AE85_BC8D9D861845 */
