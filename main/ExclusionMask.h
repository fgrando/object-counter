#ifndef A53FBDCD_9F00_4796_A6F7_D1527168876E
#define A53FBDCD_9F00_4796_A6F7_D1527168876E

#include "Mask.h"

class ExclusionMask : public Mask{
public:
    ExclusionMask() : Mask() {}
    virtual ~ExclusionMask() {}

    inline cv::Scalar getBackground() { return cv::Scalar::all(255);   }
    inline cv::Scalar getForeground() { return cv::Scalar::all(0); }
    inline std::string getWindowTitle() { return "ExclusionMask"; }
};

#endif /* A53FBDCD_9F00_4796_A6F7_D1527168876E */
