#ifndef B3991F05_2C99_44B8_949E_4FC789BCFFDD
#define B3991F05_2C99_44B8_949E_4FC789BCFFDD

static const int RETURN_BADARGS = -1;
static const int RETURN_BADFILE = -2;
static const int RETURN_BADFRAME = -3;

void drawPolys(Mat& src, Mat& dst, Scalar color=Scalar(0, 0, 255)){
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = src.clone();
    cv::findContours(contourOutput, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point> > contours_poly(contours.size());

    for( size_t i = 0; i< contours.size(); i++ )
    {
        approxPolyDP(contours[i], contours_poly[i], 3, true );
        drawContours(dst, contours_poly, (int)i, color);
    }
}

void drawCircles(Mat& src, Mat& dst, Scalar color=Scalar(0, 0, 255)){
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = src.clone();
    cv::findContours(contourOutput, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point> > contours_poly(contours.size());

    vector<Point2f>centers(contours.size());
    vector<float>radius(contours.size());

    for( size_t i = 0; i< contours.size(); i++ )
    {
        approxPolyDP(contours[i], contours_poly[i], 3, true );
        minEnclosingCircle(contours_poly[i], centers[i], radius[i]);
        circle( dst, centers[i], (int)radius[i], color, 1 );
    }
}

void drawRects(Mat& src, Mat& dst, Scalar color=Scalar(0, 0, 255)){
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = src.clone();
    cv::findContours(contourOutput, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point> > contours_poly(contours.size());

    vector<Rect> boundRect(contours.size());

    for( size_t i = 0; i< contours.size(); i++ )
    {
        approxPolyDP(contours[i], contours_poly[i], 3, true );
        boundRect[i] = boundingRect( contours_poly[i] );
        rectangle( dst, boundRect[i].tl(), boundRect[i].br(), color, 1 );
    }
}

#endif /* B3991F05_2C99_44B8_949E_4FC789BCFFDD */
