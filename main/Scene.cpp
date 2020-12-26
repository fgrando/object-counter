#include "Scene.h"
#include <iostream>


#include <opencv2/highgui/highgui.hpp>

#include "DrawPolygonCmd.h"

//#include "mtrack.h"

#include "BasicProcessing.h"
#include "Scene.h"

using namespace cv;
using namespace std;

Scene::Scene()
{
    DrawPolygonCmd* excludedAreas = new DrawPolygonCmd('e', Scalar(0, 0, 255));
    DrawPolygonCmd* monitoredAreas = new DrawPolygonCmd('m', Scalar(0, 255, 0));
    
    excludedAreas->addMask(&m_exclusionMask);
    monitoredAreas->addMask(&m_discoveryMask);
    
    m_commands.push_back(excludedAreas);
    m_commands.push_back(monitoredAreas);
}



void Scene::readMouse(int event, int x, int y){
    m_mouseEvent = event;
    m_mouseX = x;
    m_mouseY = y;

    m_userInput.event = event;
    m_userInput.x = x;
    m_userInput.y = y;
    
}

void Scene::readKeyboard(int key){
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
    

    // surge ignore noise
   // if (contours.size() > 10) { return; }

    for( size_t i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( contours[i], contours_poly[i], 3, true );
        boundRect[i] = boundingRect( contours_poly[i] );
        minEnclosingCircle(contours_poly[i], centers[i], radius[i]);

        Blob temp = {0};
        temp.age = 0;
        temp.id = UUID::next();
        temp.pos = { centers[i], radius[i] };
        temp.timestamp = TimeSource::now();
        temp.rect =  boundingRect( contours_poly[i] );

        double area = contourArea(contours[i]);

        if (area > 400)
            COP::getInstance().update(temp);
    }



    vector<Moments> mu(contours.size() );
    for( size_t i = 0; i < contours.size(); i++ )
    {
        mu[i] = moments( contours[i] );
    }
    vector<Point2f> mc( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ )
    {
        //add 1e-5 to avoid division by zero
        mc[i] = Point2f( static_cast<float>(mu[i].m10 / (mu[i].m00 + 1e-5)),
                         static_cast<float>(mu[i].m01 / (mu[i].m00 + 1e-5)) );
    }
    for( size_t i = 0; i< contours.size(); i++ )
    {
        //circle(dst, mc[i], 4, Scalar(255,0,255), -1 );
    }



    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar(0, 255, 255);
        //drawContours( dst, contours_poly, (int)i, Scalar(0, 255, 0) );
        //rectangle( dst, boundRect[i].tl(), boundRect[i].br(), color, 1 );
        //circle( dst, centers[i], (int)radius[i], Scalar(0, 255, 255), 1 );
    }
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
    lines.push_back("m....Add monitore zone");
    lines.push_back("s....Save configs to file");
    lines.push_back("q....Exit the program");
    
    for(auto l : lines){
        putText(body, l, origin, FONT_HERSHEY_SIMPLEX, scale , color);
        origin.y += space;
    }

    imshow("help", body);
}

void Scene::motionTest(cv::Mat& frame){

    //track(frame);

    Mat result = BP::BasicProcessing::getInstance().getResult();

    Mat excluded;
    bitwise_and(result, m_exclusionMask.get(), excluded);

 //   imshow("excluded", excluded);

    //Mat monitored;
    //bitwise_and(excluded, m_roiMask->get(), monitored);
    //imshow("mon", monitored);


    //contours(blurred, frame);
    drawRects(excluded, frame);

    //Rect2d roi=selectROI("tracker",frame);

    //drawBlobs(blurred);
}
static const string TRACK ="track";
static const string EXCLUDE ="exclude";
static const string INCLUDE ="include";
static const string COUNTER ="counter";

#include <fstream>
void Scene::init(){
    string settingsFile = "/home/fgrando/Downloads/object-counter/main/areas.conf";

    std::ifstream infile(settingsFile);
    std::string line;

    while (std::getline(infile, line))
    {
        if (line.size() < 3) 
            continue;
        
        if (line[0] == '#')
            continue;

        stringstream area(line);
                
        string cmd, label, data;
        area >> cmd >> label >> data;
        

        vector<Point> poly;
        while(data != ""){
            char trash = 0;
            int x, y = 0;
            
            stringstream pair(data);
            pair >> x >> trash >> y;
            
            poly.push_back(Point(x,y));

            if (!(area >> data)) {
                break;
            }
        }

        // verbose
        cout << "[" << cmd << "]" << label << "=";
        for(auto p : poly)
            cout << p;
        cout << endl;


        if (cmd == EXCLUDE){
            m_exclusionMask.addPoly(poly);
        }

        if (cmd == INCLUDE){
            m_discoveryMask.addPoly(poly);
        }
        
        if (cmd == COUNTER){
            ImageCounter img(poly[0], poly[1], label);
            m_counters.push_back(img);
        }
    }
}

void Scene::updateFrame(Mat& frame){

    for (auto& c : m_commands){
        c->process(m_userInput);
        c->draw(frame);
    }

    m_exclusionMask.initialize(frame);
    m_exclusionMask.draw(frame, Scalar(0, 0, 255, 127));

    m_discoveryMask.initialize(frame);
    m_discoveryMask.draw(frame, Scalar(0, 255, 0, 127));
    
    motionTest(frame);

    COP::getInstance().maintain();

    COP::getInstance().draw(frame);

    for(auto& ctr : m_counters){
        TrackVector blobs = COP::getInstance().crossed(ctr.m_start, ctr.m_end, ctr.crossedBlobs);
        ctr.m_counter = ctr.crossedBlobs.size();

        for(auto& b : blobs){
            cout << b.id << " crossed " << ctr.m_label << endl;
            ctr.crossedBlobs.push_back(b);
        }
        ctr.draw(frame);
    }
}
