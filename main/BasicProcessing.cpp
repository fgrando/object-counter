#include "BasicProcessing.h"

using namespace BP;

BasicProcessing& BasicProcessing::getInstance(){
    static BasicProcessing instance;
    return instance;
}

void BasicProcessing::initialize(Parameters& params){
    m_params = params;

    if (m_params.bgSubtractor == KNN){
        m_bgSubtractor = createBackgroundSubtractorKNN();
    
    } else {
        m_bgSubtractor = createBackgroundSubtractorMOG2();
    }

    

}

void smallDilation(Mat& frame){
    Mat temp = frame.clone();
    int dilationSize = 1;
    int type = MORPH_ELLIPSE;
    Mat element = getStructuringElement(type,
                                        Size(2*dilationSize + 1, 2*dilationSize+1),
                                        Point(dilationSize, dilationSize));
    dilate(temp, frame, element);

    if (0)
        imshow("smalldilation", frame);
}

void BasicProcessing::process(Mat& frame){
    static Mat foreground;

    Mat grayscale;
    cvtColor(frame, grayscale, COLOR_RGB2GRAY);

    blur(grayscale);

    m_bgSubtractor->apply(grayscale, foreground);
    //imshow("MOG", foreground);

    binarize(foreground);
    
    smallDilation(foreground);

    erosion(foreground);

    //dilation(foreground);


    //imshow("BasicProcessing", foreground);

    m_result = foreground;
}


const Mat& BasicProcessing::getResult(){
    return m_result;
}



BasicProcessing::BasicProcessing() {
    m_params.bgSubtractor = MOG2;
    m_params.bgSubVerbose = false;

    m_params.blur = 3;
    m_params.blurVerbose = false;

    m_params.erosion = ELLIPSE;
    m_params.erosionSize = 7;
    m_params.erosionVerbose = false;

    m_params.dilation = RECT;
    m_params.dilationSize = 1;
    m_params.dilationVerbose = false;

    m_params.threshold = 9;
    m_params.thresholdVerbose = false;
}


void BasicProcessing::blur(Mat& frame){
    Mat temp = frame.clone();
    GaussianBlur(temp, frame, Size(m_params.blur, m_params.blur), 0);

    if (m_params.blurVerbose)
        imshow("blur", frame);
}


void BasicProcessing::binarize(Mat& frame){
    Mat temp = frame.clone();
    threshold(temp, frame, m_params.threshold, 255, THRESH_BINARY);

    //adaptiveThreshold(temp, frame, 255,
    //    ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, m_params.threshold, 0);
    
    if (m_params.thresholdVerbose)
        imshow("binarize", frame);
}


void BasicProcessing::dilation(Mat& frame){
    Mat temp = frame.clone();

    int type = 0;
    if(RECT == m_params.dilation)        {type = MORPH_RECT;}
    else if(CROSS == m_params.dilation)  {type = MORPH_CROSS;}
    else if(ELLIPSE == m_params.dilation){type = MORPH_ELLIPSE;}
    
    Mat element = getStructuringElement(type,
                                        Size(2*m_params.dilationSize + 1, 2*m_params.dilationSize+1),
                                        Point(m_params.dilationSize, m_params.dilationSize));
    dilate(temp, frame, element);

    if (m_params.dilationVerbose)
        imshow("dilation", frame);
}


void BasicProcessing::erosion(Mat& frame){
    Mat temp = frame.clone();

    int type = 0;
    if(RECT == m_params.erosion)        {type = MORPH_RECT;}
    else if(CROSS == m_params.erosion)  {type = MORPH_CROSS;}
    else if(ELLIPSE == m_params.erosion){type = MORPH_ELLIPSE;}
    
    Mat element = getStructuringElement(type,
                                        Size(2*m_params.erosionSize + 1, 2*m_params.erosionSize+1),
                                        Point(m_params.erosionSize, m_params.erosionSize));
    erode(temp, frame, element);
    
    if (m_params.erosionVerbose)
        imshow("erosion", frame);
}


BasicProcessing::~BasicProcessing() { }