#ifndef F65368A7_4242_4A94_A375_EBD9D531F366
#define F65368A7_4242_4A94_A375_EBD9D531F366

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <vector>

using namespace cv;

class FilterROI{
public:
    inline static FilterROI& instance() { static FilterROI instance; return instance; }

    inline void filter(Mat& frame){
        Mat overlay;
        double alpha = 0.9;
        std::vector<Point> poly;

        poly.push_back(Point(0,230));
        poly.push_back(Point(121,321));
        poly.push_back(Point(578,324));
        poly.push_back(Point(623,181));
        poly.push_back(Point(704,282));
        poly.push_back(Point(892,176));
        poly.push_back(Point(891,503));
        poly.push_back(Point(0,508));

        // copy the source image to an overlay
        frame.copyTo(overlay);

        const Point *pts = (const Point*) Mat(poly).data;
        int npts = Mat(poly).rows;

        fillPoly(overlay, &pts, &npts, 1, Scalar(0,0,0), LINE_8);

        cv::addWeighted(overlay, 1.0, frame, 0, 0, frame);

        imshow("frame", frame);
    }
};

#endif /* F65368A7_4242_4A94_A375_EBD9D531F366 */
