#ifndef A14D1BB0_FE68_4B91_9312_C3DEF499B19A
#define A14D1BB0_FE68_4B91_9312_C3DEF499B19A

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

using namespace cv;

#define MINBRIGHNESS 100

class OpticalFlowBlobs{
public:
    inline static OpticalFlowBlobs& instance() { static OpticalFlowBlobs instance; return instance; }

    inline void init(Mat& frame){
        cvtColor(frame, m_prvs, COLOR_BGR2GRAY);
    }

    inline void getBlobs(Mat& frame, Mat& blobs) {
        Mat next;
        cvtColor(frame, next, COLOR_BGR2GRAY);

        if (m_prvs.empty())
            m_prvs = next; // first time only

        Mat flow(m_prvs.size(), CV_32FC2);
        calcOpticalFlowFarneback(m_prvs, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

        // visualization
        Mat flow_parts[2];
        split(flow, flow_parts);
        Mat magnitude, angle, magn_norm;
        cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
        normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);

        Mat clouds, mask, colormask, res;
        magn_norm.convertTo(clouds, CV_8U, 255.0);

        // keep only brightest flows
        threshold(clouds, mask, MINBRIGHNESS, 255, THRESH_BINARY);

        //addWeighted(next, 0.2, mask, 0.8, 0, blobs);
        blobs = mask;

        m_prvs = next;
    }

    Mat m_prvs; // previous frame
};

#endif /* A14D1BB0_FE68_4B91_9312_C3DEF499B19A */
