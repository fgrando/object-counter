#include "Control.h"
#include <iostream>

using namespace Detector;

#include <opencv2/highgui/highgui.hpp>

#include "DrawPolygonCmd.h"

using namespace cv;
using namespace std;

Control::Control() : m_input(nullptr) {

    m_mask = new Mask();
    
    DrawPolygonCmd* cmd = new DrawPolygonCmd();
    
    cmd->addMask(m_mask);
    
    m_commands.push_back(cmd);
}

void Control::readMouse(int event, int x, int y){
    m_mouseEvent = event;
    m_mouseX = x;
    m_mouseY = y;

    m_userInput.event = event;
    m_userInput.x = x;
    m_userInput.y = y;
    
}

void Control::readKeyboard(int key){
    m_kbdr = key;
    m_userInput.kbdr = key;

    if ((char)key == 'q'){
        exit(0);
    }
}

#include "persistence.h"
#include <opencv2/core/version.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/tracking.hpp>

void moveDetection(Mat& frame){
    static Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorMOG2();
    //pBackSub = createBackgroundSubtractorKNN();
    static Mat background;


    pBackSub->apply(frame, background);


    //get the frame number and write it on the current frame
    rectangle(frame, cv::Point(10, 2), cv::Point(100,20),
              cv::Scalar(255,255,255), -1);
    stringstream ss;
    ss << "test FPS"; //capture.get(CAP_PROP_POS_FRAMES);
    string frameNumberString = ss.str();
    putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
            FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));

    imshow("FG Mask", background);


#if 0
    Mat grayscale;
    cvtColor(frame, grayscale, COLOR_RGB2GRAY);
//    imshow("grayscale", grayscale);
    //gray = cv2.GaussianBlur (gray, (21, 21), 0)

    Mat blurred;
    int coef = 9;
    GaussianBlur(grayscale, blurred, Size(coef, coef), 0);
    imshow("blurred", blurred);

    if (!initialized)
    {
        background = blurred.clone();
        initialized = true;
    }

    // subtract
    Mat subtracted;
    absdiff(background, blurred, subtracted);
    imshow("subtracted", subtracted);


    Mat thresholded;
    threshold(subtracted, thresholded, 25, 255, THRESH_BINARY);
    imshow("threshold", thresholded);


    Mat dilated;
    int dilation_type = 0;
    int dilation_size = 1;
    int dilation_elem = 2;
    if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
    else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
    else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }
    Mat element = getStructuringElement( dilation_type,
                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                       Point( dilation_size, dilation_size ) );
    dilate(thresholded, dilated, element);
    imshow("dilated", dilated);



    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = dilated.clone();
    cv::findContours( contourOutput, contours, RETR_LIST, CHAIN_APPROX_SIMPLE );

    //Draw the contours
    cv::Mat contourImage(frame.size(), CV_8UC3, cv::Scalar(0,0,0));
    cv::Scalar colors[3];
    colors[0] = cv::Scalar(255, 0, 0);
    colors[1] = cv::Scalar(0, 255, 0);
    colors[2] = cv::Scalar(0, 0, 255);
    for (size_t idx = 0; idx < contours.size(); idx++) {
        cv::drawContours(contourImage, contours, idx, colors[idx % 3]);
    }
    imshow("countourns", contourImage);


    background = blurred;
#endif
}

void blur(Mat& src, Mat& dst){
    GaussianBlur(src, dst, Size(db::blur, db::blur), 0);
}



static Ptr<SimpleBlobDetector> d = SimpleBlobDetector::create();
// Set up the detector with default parameters.
void drawBlobs(Mat& src){

    SimpleBlobDetector::Params par;
    par.thresholdStep = 10;
    par.minThreshold = 50;
    par.maxThreshold = 220;
    par.minRepeatability = 2;
    par.minDistBetweenBlobs = 10;
    par.filterByColor = true;
    par.blobColor = 0;
    par.filterByArea = true;
    par.minArea = 25;
    par.maxArea = 5000;
    par.filterByCircularity = false;
    par.minCircularity = 0.8f;
    par.maxCircularity = std::numeric_limits<float>::max();
    par.filterByInertia = true;
    par.minInertiaRatio = 0.1f;
    par.maxInertiaRatio = std::numeric_limits<float>::max();
    par.filterByConvexity = true;
    par.minConvexity = 0.95f;
    par.maxConvexity = std::numeric_limits<float>::max();



    // Detect blobs.
    std::vector<KeyPoint> keypoints;
    d->detect(src, keypoints);

    // Draw detected blobs as red circles.
    // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
    Mat im_with_keypoints;
    drawKeypoints(src, keypoints, im_with_keypoints, Scalar(0,0,255),
        DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    // Show blobs
    imshow("keypoints", im_with_keypoints);
}

void dilation(Mat& src, Mat& dst){
    int dilation_type = 0;
    if(db::dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
    else if(db::dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
    else if(db::dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }
    
    Mat element = getStructuringElement(dilation_type,
                       Size( 2*db::dilation_size + 1, 2*db::dilation_size+1),
                       Point(db::dilation_size, db::dilation_size) );
    
    dilate(src, dst, element);
    imshow("dilated", dst);
}

void threshd(Mat& src, Mat& dst){
    //threshold(src, dst, 1, 255, THRESH_BINARY);

    adaptiveThreshold(src,dst,255,
        ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,db::tresh_adap_blob,0);

    imshow("threshold", dst);
}

void erosion(Mat& src, Mat& dst)
{
  int erosion_type = 0;
  if(db::erosion_elem == 0 ){erosion_type = MORPH_RECT;}
  else if(db::erosion_elem == 1 ){erosion_type = MORPH_CROSS;}
  else if(db::erosion_elem == 2) {erosion_type = MORPH_ELLIPSE;}
  Mat element = getStructuringElement( erosion_type,
                       Size( 2*db::erosion_size + 1, 2*db::erosion_size+1 ),
                       Point(db::erosion_size, db::erosion_size));
  
  erode(src, dst, element );
  imshow("erosion", dst);
}

void contours(Mat& src, Mat& dst){
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = src.clone();
    cv::findContours( contourOutput, contours, RETR_LIST, CHAIN_APPROX_SIMPLE );

    //Draw the contours
    cv::Mat contourImage(dst.size(), CV_8UC3, cv::Scalar(0,0,0));
    cv::Scalar colors[3];
    colors[0] = cv::Scalar(255, 0, 0);
    colors[1] = cv::Scalar(0, 255, 0);
    colors[2] = cv::Scalar(0, 0, 255);
    Scalar color = cv::Scalar(0, 0, 255);
    for (size_t idx = 0; idx < contours.size(); idx++) {
        //cv::drawContours(contourImage, contours, idx, colors[idx % 3]);
        //cv::drawContours(contourImage, contours, idx, color);
        cv::drawContours(dst, contours, idx, color);
    }
    //imshow("countourns", contourImage);
}

void drawRects(Mat& src, Mat& dst){
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = src.clone();
    cv::findContours( contourOutput, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );

    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>centers( contours.size() );
    vector<float>radius( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( contours[i], contours_poly[i], 3, true );
        boundRect[i] = boundingRect( contours_poly[i] );
        minEnclosingCircle( contours_poly[i], centers[i], radius[i] );
    }

    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( 255, 0, 0);
        drawContours( dst, contours_poly, (int)i, Scalar(0, 255, 0) );
        rectangle( dst, boundRect[i].tl(), boundRect[i].br(), color, 1 );
        circle( dst, centers[i], (int)radius[i], Scalar(0, 255, 255), 1 );
    }
}

void text(Mat& frame, string text){
    //get the frame number and write it on the current frame
    rectangle(frame, cv::Point(db::text_x, db::text_y), cv::Point(100,20),
              cv::Scalar(240,240,240), -1);
    
    stringstream ss;
    ss << text;
    
    string frameNumberString = ss.str();
    putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
            FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
}


void showHelp(){
    float scale = 0.5;
    int space = 20;
    cv::Scalar color(0,0,0);
    cv::Point origin(10, 20);
    cv::Size sz(200, 250);
    cv::Mat body(sz, CV_8UC3, cv::Scalar(255,255,255));

    vector<string> lines;
    
    lines.push_back("Abailable commands:");
    lines.push_back("h....Show/hide this help");
    lines.push_back("p....Pause");
    lines.push_back("c....Continue");
    lines.push_back("e....Add exclusion zone");
    lines.push_back("s....Save configs to file");
    lines.push_back("q....Exit the program");
    
    for(auto l : lines){
        putText(body, l, origin, FONT_HERSHEY_SIMPLEX, scale , color);
        origin.y += space;
    }

    imshow("help", body);
}

extern Ptr<BackgroundSubtractor> BACKSUB;
void motionTest(Mat& frame){
    showHelp();

    static Mat FGMASK;

    Mat grayscale;
    cvtColor(frame, grayscale, COLOR_RGB2GRAY);

    Mat blurred1;
    blur(grayscale, blurred1);

    //update the background model
    BACKSUB->apply(blurred1, FGMASK);
    imshow("FG Mask", FGMASK);

    Mat thresholded;
    threshd(FGMASK, thresholded);

    Mat eroded;
    erosion(thresholded, eroded);

    Mat dilated;
    dilation(eroded, dilated);

    Mat blurred;
    blur(dilated, blurred);


    //contours(blurred, frame);
    drawRects(blurred, frame);

    text(frame, "test!@");
    imshow("finished", frame);

    //Rect2d roi=selectROI("tracker",frame);

    drawBlobs(blurred);
}

void Control::updateFrame(Mat& frame){

#if 0
    m_input = &DetectorFSM::getInstance();

    if (m_input != nullptr)
    {
        m_input->process(m_mouseEvent, m_mouseX, m_mouseY, m_kbdr);

        m_input->Draw(frame);

        m_input->Persist(m_lines);
    }

    for(int i=0; i <  m_lines.size(); i++){
        cv::Point& a = m_lines[i].a;
        cv::Point& b = m_lines[i].b;

        cv::Scalar colorLine(0,255,255);
        int thicknessLine = 2;
        cv::line(frame, a, b, colorLine, thicknessLine);
    }

    //moveDetection(frame);
    motionTest(frame);
#endif

    for (auto& c : m_commands){
        c->process(m_userInput);
        c->draw(frame);
    }

    m_mask->draw(frame);

}
