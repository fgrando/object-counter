#ifndef DFA62C5C_80BF_4AD7_B436_3763CC7DBD10
#define DFA62C5C_80BF_4AD7_B436_3763CC7DBD10

#include "Mask.h"

class DiscoveryMask : public Mask{
public:
    DiscoveryMask() : Mask() {}
    virtual ~DiscoveryMask() {}

    inline cv::Scalar getBackground() { return cv::Scalar::all(0);   }
    inline cv::Scalar getForeground() { return cv::Scalar::all(255); }
    inline std::string getWindowTitle() { return "DiscoveryMask"; }
};

#endif /* DFA62C5C_80BF_4AD7_B436_3763CC7DBD10 */
