#ifndef A42EBBD8_0DC9_4327_AD95_0129CA08A219
#define A42EBBD8_0DC9_4327_AD95_0129CA08A219

#include <string>
#include <opencv2/imgproc/imgproc.hpp>

#include "DetectorFSM.h"


#include "persistence.h"

#include <vector>
using namespace std;

class Control{
public:
    Control();

    inline string getPictureSource() {return "picture.jpg";}
    inline string getVideoSource() {return "0";}

    inline string getWindowName() {return "CAM0";}

    void readMouse(int event, int x, int y);
    
    void readKeyboard(int key);
    
    void updateFrame(cv::Mat& frame);

private:
    int m_mouseX, m_mouseY, m_mouseEvent, m_kbdr;

    Detector::DetectorFSM* m_input;

    
    vector<Line> m_lines;
};

#endif /* A42EBBD8_0DC9_4327_AD95_0129CA08A219 */
