#include "COP.h"

#include "TimeSource.h"

#include "persistence.h"

#include <iostream>

using namespace std;


COP& COP::getInstance(){
    static COP instance;
    return instance;
}

bool COP::moved(const Blob& a, const Blob&b) {
    static const double thresholdPx = 100;
    
    Point2f difference = a.pos.center - b.pos.center;
    double distance = sqrt(difference.ddot(difference));
    
    return (distance < thresholdPx);
}

void COP::update(const Blob& b){

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


void COP::draw(Mat& frame){
    for(auto& blob : m_tracks){
        blob.draw(frame);
    }
}


void COP::maintain(){
    Timestamp now = TimeSource::now();

    for(TrackVector::iterator it = m_tracks.begin();
        it != m_tracks.end();
        /* increment in loop*/ )
    {
        if (it->expired(now)) {
            it = m_tracks.erase(it);
//            cout << "erased " << it->id << " " << it->age << endl;
        } else {
            ++it;
        }
    }
}

TrackVector COP::crossed(Point a, Point b, const TrackVector& ignored){
    //determinant of 
    // | x2-x1  x3-x1 |
    // | y2-y1  y3-y1 |
    
    TrackVector blobs;

    // check every blob to know if crossed the line
    for(auto& blob : m_tracks){

        bool ignoreThisBlob = false;
        for(const auto& ignore : ignored) {
            if (blob.id == ignore.id){
                ignoreThisBlob = true;
                break;
            }
        }

        if (ignoreThisBlob) continue;

        if (blob.path.size() < DB::get().detectionMinPathLenToBeDetected) continue;

        bool crossed = false;

        Point t = blob.path[0];    
        int lastDir = ( (b.x-a.x) * (t.y-a.y) ) - ( (b.y - a.y) * (t.x-a.x) );
        for(auto& pts : blob.path) {
            t = pts;
            int dir = ( (b.x-a.x) * (t.y-a.y) ) - ( (b.y - a.y) * (t.x-a.x) );
            
            if (((dir > 0) && (lastDir < 0)) || 
                ((dir < 0) && (lastDir > 0))){
                crossed = true;
                break;
            }
            lastDir = dir;
        }

        if (crossed){
            blobs.push_back(blob);
        }

    }
    
    return blobs;
}


COP::COP(){}
COP::~COP(){}