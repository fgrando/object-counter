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

extern bool VERBOSE;

#define CONST_detectionLifetimeMs 5000 // optical flow takes time to process
#define CONST_detectionAgeToCheckDistanceFrames 40
#define CONST_detectionPathDistancePx 60

#define PINK Scalar(255,0,255)
#define CYAN Scalar(255,255,0)
#define BLACK Scalar(0,0,0)


struct Blob {

    struct Position {
        Point2f  center;
        float    radius;
    };

    Blob() {
        color = PINK;
        startFrame = endFrame = 0;
        id = age = 0;
        pos.center = {0};
        pos.radius = 0;
        distancePx = 0.0;
        path.clear();
        saved = false;
    }

    unsigned long long startFrame;
    unsigned long long endFrame;
    uint64_t id;
    uint64_t age; // frames
    Position pos;
    Timestamp timestamp;
    Rect rect;
    double distancePx;
    Scalar color;
    bool saved;
    int bestFrame;
    int bestDistance;

    std::vector<cv::Point> path;

    inline void update(const Position& position) {
        path.push_back(pos.center);
        pos.center = position.center;
        pos.radius = position.radius;
        timestamp = TimeSource::now();
        age++;

        Point2f difference = Point(position.center.x, position.center.y) - path.back();
        distancePx += sqrt(difference.ddot(difference));

        Point2f diferenceCenter = Point(position.center.x, position.center.y) - VideoSrc::instance().center();
        double disttanceCenter=  sqrt(difference.ddot(difference));
        if (disttanceCenter < bestDistance){
            bestDistance = disttanceCenter;
            bestFrame = VideoSrc::instance().count();
        }

        color = PINK;
    }

    inline void update(const Rect& r) {
        color = PINK;
        rect = r;
    }

    inline void draw(Mat& frame) {
        static const Scalar black(0,0,0);
        static const Scalar green(0,255,0);
        static const Scalar white(255,255,255);
        static const int thickness = 1;
        static const float scale = 0.4;

        std::string text = std::to_string(id) +
                           "(" + std::to_string(age) + "f) " +
                           "(" + std::to_string((int)rect.area()) + "m)";

        circle(frame, pos.center, pos.radius, color, thickness);
        //rectangle(frame, rect, white, thickness);

        // add label:
        Point2f label = pos.center;
        label.y+=pos.radius;

        putText(frame, text, label, FONT_HERSHEY_SIMPLEX, scale , black, 3);
        putText(frame, text, label, FONT_HERSHEY_SIMPLEX, scale , white);

        if (VERBOSE) {
            const Point *pts = (const Point*) Mat(path).data;
            int npts = Mat(path).rows;
            polylines(frame, &pts, &npts, 1, false, green);
        }

        color = BLACK; // this will change if this blob receives an update
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
