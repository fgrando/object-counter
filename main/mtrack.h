#ifndef C32A4EF4_B249_443A_8A28_6CE7025B35FD
#define C32A4EF4_B249_443A_8A28_6CE7025B35FD

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <vector>

using namespace cv;
using namespace std;

vector<string> trackerTypes = {"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"};

// create tracker by name
inline Ptr<Tracker> createTrackerByName(string trackerType)
{
  Ptr<Tracker> tracker;
  if (trackerType ==  trackerTypes[0])
    tracker = TrackerBoosting::create();
  else if (trackerType == trackerTypes[1])
    tracker = TrackerMIL::create();
  else if (trackerType == trackerTypes[2])
    tracker = TrackerKCF::create();
  else if (trackerType == trackerTypes[3])
    tracker = TrackerTLD::create();
  else if (trackerType == trackerTypes[4])
    tracker = TrackerMedianFlow::create();
  else if (trackerType == trackerTypes[5])
    tracker = TrackerGOTURN::create();
  else if (trackerType == trackerTypes[6])
    tracker = TrackerMOSSE::create();
  else if (trackerType == trackerTypes[7])
    tracker = TrackerCSRT::create();
  else {
    cout << "Incorrect tracker name" << endl;
    cout << "Available trackers are: " << endl;
    for (vector<string>::iterator it = trackerTypes.begin() ; it != trackerTypes.end(); ++it)
      std::cout << " " << *it << endl;
  }
  return tracker;
}

// Fill the vector with random colors
inline void getRandomColors(vector<Scalar>& colors, int numColors)
{
  RNG rng(0);
  for(int i=0; i < numColors; i++)
    colors.push_back(Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255)));
}

inline void track(Mat& frame){
    static vector<Rect> bboxes;
    static bool init = false;
    static vector<Scalar> colors;
    static Ptr<MultiTracker> multiTracker = cv::MultiTracker::create();

    if (!init)
    {
        init = true;

        // Get bounding boxes for first frame
        // selectROI's default behaviour is to draw box starting from the center
        // when fromCenter is set to false, you can draw box starting from top left corner
        bool showCrosshair = true;
        bool fromCenter = false;
        cout << "\n==========================================================\n";
        cout << "OpenCV says press c to cancel objects selection process" << endl;
        cout << "It doesn't work. Press Escape to exit selection process" << endl;
        cout << "\n==========================================================\n";
        //cv::selectROIs("MultiTracker", frame, bboxes, showCrosshair, fromCenter);


        bboxes.push_back(Rect(Point(365,302),Point(400,326)));


        // quit if there are no objects to track
        if(bboxes.size() < 1)
          return;

        getRandomColors(colors, bboxes.size());



        // Specify the tracker type
        string trackerType = "BOOSTING";
        // Create multitracker

        // Initialize multitracker
        for(int i=0; i < bboxes.size(); i++)
          multiTracker->add(createTrackerByName(trackerType), frame, Rect2d(bboxes[i]));

        cout << "initialized" << endl;

    }


     //Update the tracking result with new frame
     multiTracker->update(frame);

      Mat clon = frame.clone();
     // Draw tracked objects
     for(unsigned i=0; i<multiTracker->getObjects().size(); i++)
     {
       rectangle(clon, multiTracker->getObjects()[i], colors[i], 2, 1);
     }

     // Show frame
     imshow("MultiTracker", clon);
}


#endif /* C32A4EF4_B249_443A_8A28_6CE7025B35FD */
