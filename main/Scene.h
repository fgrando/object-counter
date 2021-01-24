#ifndef A42EBBD8_0DC9_4327_AD95_0129CA08A219
#define A42EBBD8_0DC9_4327_AD95_0129CA08A219

#include <string>
#include <opencv2/imgproc/imgproc.hpp>

#include "command.h"

#include "persistence.h"

#include "Mask.h"

#include <vector>
#include <memory>

#include "COP.h"
#include "ImageCounter.h"
#include "ExclusionMask.h"
#include "DiscoveryMask.h"

using namespace std;

class Scene{
public:
    Scene();

    inline string getPictureSource() {return "picture.jpg";}
    inline string getVideoSource() {return "0";}

    inline string getWindowName() {return "CAM0";}

    void readMouse(int event, int x, int y);

    void readKeyboard(int key);

    void updateFrame(cv::Mat& frame);

    void init();

    void finish();
private:
    void motionTest(cv::Mat& frame);
    int m_mouseX, m_mouseY, m_mouseEvent, m_kbdr;

    UserInput m_userInput;

    vector<ImageCounter> m_counters;

    vector<Command*> m_commands;

    ExclusionMask m_exclusionMask;

    DiscoveryMask m_discoveryMask;


};

#endif /* A42EBBD8_0DC9_4327_AD95_0129CA08A219 */
