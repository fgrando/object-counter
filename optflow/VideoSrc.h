#ifndef ACE47ABA_7A37_4D21_AE85_BC8D9D861845
#define ACE47ABA_7A37_4D21_AE85_BC8D9D861845

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

    inline Point center() { return m_center; }

    inline bool setCapture(std::string filename) {
        m_name = filename;
        m_capture = VideoCapture(samples::findFile(filename));
        m_height = m_capture.get(CAP_PROP_FRAME_HEIGHT);
        m_width = m_capture.get(CAP_PROP_FRAME_WIDTH);
        m_total = m_capture.get(CAP_PROP_FRAME_COUNT);
        m_center = Point((m_width*ResizeFactor)/2, (m_height*ResizeFactor)/2);
        return m_capture.isOpened();
    }

    inline unsigned long long count() {
        unsigned long long frameidx = m_capture.get(CAP_PROP_POS_FRAMES);
        return frameidx;
    }

    inline std::string name() { return m_name; }

    inline unsigned int total() { return m_total; }

    inline unsigned int width() { return m_width; }

    inline unsigned int height() { return m_height; }

    inline bool get(Mat& frame){
        m_capture >> m_frame;

        if (!m_frame.empty())
            cv::resize(m_frame, frame, cv::Size(), ResizeFactor, ResizeFactor);

        return (!m_frame.empty());
    }

    inline bool get(Mat& frame, int idx){
        m_capture.set(CAP_PROP_POS_FRAMES, idx);
        m_capture >> m_frame;

//        if (!m_frame.empty())
//            cv::resize(m_frame, frame, cv::Size(), ResizeFactor, ResizeFactor);

        return (!m_frame.empty());
    }

    inline static VideoSrc& instance() { static VideoSrc instance; return instance; }

private:
    VideoCapture m_capture;
    std::string m_name;
    Mat m_frame;
    Point m_center;
    unsigned int m_height;
    unsigned int m_width;
    unsigned int m_total;
};

#endif /* ACE47ABA_7A37_4D21_AE85_BC8D9D861845 */
