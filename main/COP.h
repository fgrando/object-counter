#ifndef CDB9704C_AF86_455E_AE22_8EF55A2D0B30
#define CDB9704C_AF86_455E_AE22_8EF55A2D0B30

#include "Blob.h"
#include <vector>

typedef  std::vector<Blob> TrackVector;

class COP {
public:

    static COP& getInstance();

    void update(const Blob& b);

    virtual ~COP();

    void draw(Mat& frame);

    void maintain();

    TrackVector crossed(Point a, Point b, const TrackVector& ignored);

private:
    COP();


    bool moved(const Blob& a, const Blob&b);

    TrackVector m_tracks;
};



#endif /* CDB9704C_AF86_455E_AE22_8EF55A2D0B30 */
