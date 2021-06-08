#ifndef ACBFD6E8_F2B8_4C4C_89C8_5588D6382665
#define ACBFD6E8_F2B8_4C4C_89C8_5588D6382665

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cstdint>
#include <string>

#include "TimeSource.h"

#include <iostream>
using namespace std;

// situational picture
using namespace cv;


#define CONST_detectionLifetimeMs 100
#define CONST_detectionAgeToCheckDistanceFrames 40
#define CONST_detectionPathDistancePx 80

struct Blob {

    struct Position {
        Point2f  center;
        float    radius;
    };

    unsigned long long startFrame;
    unsigned long long endFrame;
    uint64_t id;
    uint64_t age; // frames
    Position pos;
    Timestamp timestamp;
    Rect rect;
    double distancePx;
    Mat thumb;

    std::vector<cv::Point> path;

    inline void update(const Position& position) {
        path.push_back(pos.center);
        pos.center = position.center;
        pos.radius = position.radius;
        timestamp = TimeSource::now();
        age++;

        Point2f difference = Point(position.center.x, position.center.y) - path.back();
        distancePx += sqrt(difference.ddot(difference));
    }

    inline void update(const Rect& r) {
        rect = r;
    }

    inline void draw(Mat& frame) {
            static const Scalar blue(255,0,0);
            static const Scalar yellow(128,255,255);
            static const Scalar cyan(255,255,0);
            static const int thickness = 1;
            static const float scale = 0.4;

            std::string text = std::to_string(id) +
                               "(" + std::to_string(age) + "f) " +
                               "(" + std::to_string((int)rect.area()) + "m)";

            circle(frame, pos.center, pos.radius, cyan, thickness);
            //rectangle(frame, rect, yellow, thickness);

            // add label:
            Point2f label = pos.center;
            label.y+=pos.radius;

            int charHeigthPx = 13;
            int charWidthPx = 7;
            label.y-=charHeigthPx;
            Rect textBG(label, Size(text.length()*charWidthPx, charHeigthPx));
            rectangle(frame, textBG, Scalar(128,128, 128), -1);

            label.y+=charHeigthPx;
            putText(frame, text, label, FONT_HERSHEY_SIMPLEX, scale , yellow);


            const Point *pts = (const Point*) Mat(path).data;
            int npts = Mat(path).rows;
            polylines(frame, &pts, &npts, 1, false, blue);
    }

    inline bool expired(const Timestamp& t){
        std::chrono::milliseconds lifetime (CONST_detectionLifetimeMs);
        std::chrono::milliseconds delta(0);

        delta =
            std::chrono::duration_cast<std::chrono::milliseconds> (t - timestamp);

        // expire the blob if it does not move according to age
        if (age >=  CONST_detectionAgeToCheckDistanceFrames) {
            if (distancePx < CONST_detectionPathDistancePx) {
                cout << "expiring " << id << " since this blob aged but it is not moving ("
                     << distancePx << " pixels (min is " <<  CONST_detectionPathDistancePx << " pixels)" << endl;
                return true;
            }
        }

        return (delta > lifetime);
    }


};


class UUID {
public:
    static uint64_t next() {
        static uint64_t counter = 0;
        return counter++;
    }
};

#endif /* ACBFD6E8_F2B8_4C4C_89C8_5588D6382665 */
