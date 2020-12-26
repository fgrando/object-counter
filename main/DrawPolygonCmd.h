#ifndef A02858E4_5479_4A95_A043_16AA388B2366
#define A02858E4_5479_4A95_A043_16AA388B2366

#include "command.h"
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Mask.h"

class DrawPolygonCmd : public Command{

public:
    enum class State {Idle, WaitOrigin, Preview};

    explicit DrawPolygonCmd(char key, cv::Scalar color);

    ~DrawPolygonCmd();

    std::string getHelp();

    void process(const UserInput& input);
    void draw(cv::Mat& frame);

    void addMask(Mask* mask);

private:
    char m_key;
    State m_state;
    cv::Point m_tempPoint;
    std::vector<cv::Point> m_points;
    cv::Scalar m_color;

    std::vector<Mask*> m_masks;
    int m_previousEvent;
};




#endif /* A02858E4_5479_4A95_A043_16AA388B2366 */
