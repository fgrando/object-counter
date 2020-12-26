#ifndef DD7547AF_238A_42BB_BF81_0C032DC85F77
#define DD7547AF_238A_42BB_BF81_0C032DC85F77

#include <string>
#include <opencv2/core/core.hpp>

struct UserInput{
    int x;
    int y;
    int event;
    int kbdr;
};


class Command{
public:
    virtual ~Command() {}

    virtual std::string getHelp() = 0;
    virtual void draw(cv::Mat& frame) = 0;
    virtual void process(const UserInput& input) = 0;
};


#endif /* DD7547AF_238A_42BB_BF81_0C032DC85F77 */
