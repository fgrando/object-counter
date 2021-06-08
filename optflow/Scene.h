#ifndef B965D066_50AE_4F2D_99D1_DE7FB53FEC9C
#define B965D066_50AE_4F2D_99D1_DE7FB53FEC9C

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <vector>
#include <string>

#include "VideoSrc.h"
#include "Blob.h"
using namespace cv;

#define MINAREAPX 800 // current size is 0.5 short
#define MINAGETODISPLAY 2 //frames
#define BLOBAGETRIGGERSAVE (MINAGETODISPLAY+2)

typedef  std::vector<Blob> TrackVector;

class Scene{
public:
    inline static Scene& instance() { static Scene instance; return instance; }

    inline void updateBlobs(Mat& src) {
        std::vector<std::vector<cv::Point> > contours;
        cv::Mat contourOutput = src.clone();
        cv::findContours(contourOutput, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

        vector<vector<Point> > contours_poly( contours.size());

        vector<Point2f>centers( contours.size());
        vector<float>radius( contours.size());

        for(size_t i = 0; i < contours.size(); i++)
        {
            approxPolyDP( contours[i], contours_poly[i], 3, true );
            minEnclosingCircle(contours_poly[i], centers[i], radius[i]);

            Blob temp = {0};
            temp.startFrame = VideoSrc::instance().count();
            temp.age = 0;
            temp.id = UUID::next();
            temp.pos = { centers[i], radius[i] };
            temp.timestamp = TimeSource::now();
            temp.rect =  boundingRect(contours_poly[i]);

            double area = contourArea(contours[i]);

            // filter noises (small blobs)
            if (area > MINAREAPX)
                this->update(temp);
        }
    }

    inline void update(const Blob& b) {
        bool blobUpdated = false;

        // calculate distance
        for(auto& blob : m_tracks){
            if (moved(blob, b)){
                blob.update(b.pos);
                blob.update(b.rect);
                blobUpdated = true;
                break;
            }
        }

        if (!blobUpdated) {
            Blob copy = b;
            m_tracks.push_back(copy);
        }
    }

    void draw(Mat& frame){
        for(auto& blob : m_tracks){
            if (blob.age >= MINAGETODISPLAY) {
                blob.draw(frame);

                if (blob.age == MINAGETODISPLAY) {
                    // saving on deletion (so we know the path)
                    std::string file = VideoSrc::instance().name();
                    std::size_t found = file.find_last_of('.');
                    file = file.substr(0, found);

                    // insert leading zeroes
                    std::string frameStr = std::to_string(VideoSrc::instance().count());
                    int maxDigits = 7;
                    while(frameStr.length() < maxDigits) {
                        frameStr = "0" + frameStr;
                    }

                    std::string name = file + "_"  + frameStr + ".jpeg";
                    imwrite(name, frame);
                }
            }
        }
    }

    void maintain() {
        Timestamp now = TimeSource::now();

        for(TrackVector::iterator it = m_tracks.begin(); it != m_tracks.end(); /* increment in loop*/ )
        {
            if (it->expired(now)) {
                it->endFrame = VideoSrc::instance().count();

                if (it->age > BLOBAGETRIGGERSAVE) {
                    std::cout << it->id << " " << it->startFrame << " " << it->endFrame << " " << it->age << std::endl;
                }

                it = m_tracks.erase(it);
            //    cout << "erased " << it->id << " " << it->age << endl;
            } else {
                ++it;
            }
        }
    }

private:
    inline bool moved(const Blob& a, const Blob&b){
        static const double thresholdPx = 100;

        Point2f difference = a.pos.center - b.pos.center;
        double distance = sqrt(difference.ddot(difference));

        return (distance < thresholdPx);
    }


    TrackVector m_tracks;

};

#endif /* B965D066_50AE_4F2D_99D1_DE7FB53FEC9C */
