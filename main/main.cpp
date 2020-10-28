#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

#include "persistence.h"

int main(int argc, char *argv[])
{
    Mat img = imread("picture.jpg");
    if(img.empty())
    {
        std::cout << "failed to load image!\n";
        return -1;
    } 
       
    namedWindow("picture", WINDOW_AUTOSIZE );
    imshow("picture", img);
    waitKey(9000);
    return 0;
}
