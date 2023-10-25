#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int thresh = 50, N = 11;

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle( Point pt1, Point pt2, Point pt0 )
{
  double dx1 = pt1.x - pt0.x;
  double dy1 = pt1.y - pt0.y;
  double dx2 = pt2.x - pt0.x;
  double dy2 = pt2.y - pt0.y;
  return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

static void findSquares( const Mat& image, vector<vector<Point> >& squares )
{
  squares.clear();
  Mat pyr, timg, gray0(image.size(), CV_8U), gray;
  // down-scale and upscale the image to filter out the noise
  pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
  pyrUp(pyr, timg, image.size());
  vector<vector<Point> > contours;
  // find squares in every color plane of the image
  for( int c = 0; c < 3; c++ )
  {
    int ch[] = {c, 0};
    mixChannels(&timg, 1, &gray0, 1, ch, 1);
    // try several threshold levels
    for( int l = 0; l < N; l++ )
    {
      // hack: use Canny instead of zero threshold level.
      // Canny helps to catch squares with gradient shading
      if( l == 0 )
      {
        // apply Canny. Take the upper threshold from slider
        // and set the lower to 0 (which forces edges merging)
        Canny(gray0, gray, 0, thresh, 5);
        // dilate canny output to remove potential
        // holes between edge segments
        dilate(gray, gray, Mat(), Point(-1,-1));
      }
      else
      {
        // apply threshold if l!=0:
        // tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
        gray = gray0 >= (l+1)*255/N;
      }
      // find contours and store them all as a list
      findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
      vector<Point> approx;
      // test each contour
      for( size_t i = 0; i < contours.size(); i++ )
      {
        // approximate contour with accuracy proportional
        // to the contour perimeter
        approxPolyDP(contours[i], approx, arcLength(contours[i], true)*0.02, true);
        // square contours should have 4 vertices after approximation
        // relatively large area (to filter out noisy contours)
        // and be convex.
        // Note: absolute value of an area is used because
        // area may be positive or negative - in accordance with the
        // contour orientation
        if( approx.size() == 4 &&
          fabs(contourArea(approx)) > 1000 &&
          isContourConvex(approx) )
        {
          double maxCosine = 0;
          for( int j = 2; j < 5; j++ )
          {
            // find the maximum cosine of the angle between joint edges
            double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
            maxCosine = MAX(maxCosine, cosine);
          }
          // if cosines of all angles are small
          // (all angles are ~90 degree) then write quandrange
          // vertices to resultant sequence
          if( maxCosine < 0.4 )
            squares.push_back(approx);
        }
      }
    }
  }
}

int main(int, char**)
{
 Mat frame, frame_gray, frame_bw, tframe;
 //--- INITIALIZE VIDEOCAPTURE
 VideoCapture cap;

 // open the default camera using default API
 // cap.open(0);
 // OR advance usage: select any API backend
 int deviceID = 2; // 0 = open default camera
 int apiID = cv::CAP_ANY; // 0 = autodetect default API

 // open selected camera using selected API
 cap.open(deviceID, apiID);

 // check if we succeeded
 if (!cap.isOpened()) {
   cerr << "ERROR! Unable to open camera\n";
   return -1;
 }
 
 //--- GRAB AND WRITE LOOP
 cout << "Start grabbing" << endl
 << "Press any key to terminate" << endl;
 for (;;)
 {
   // wait for a new frame from camera and store it into 'frame'
    cap.read(frame);
    // check if we succeeded
    if (frame.empty()) {
      cerr << "ERROR! blank frame grabbed\n";
      break;
    }

    Point image_center(frame.cols / 2, frame.rows / 2);
    vector<vector<Point> > squares;
    vector<vector<Point> > squares2;

    //Mat pyr, timg, gray0(frame.size(), CV_8U), gray;
    //// down-scale and upscale the image to filter out the noise
    //pyrDown(frame, pyr, Size(frame.cols/2, frame.rows/2));
    //pyrUp(pyr,tframe , frame.size());

    // first filter
    findSquares(frame, squares);
    polylines(frame, squares, true, Scalar(0, 255, 0), 3, LINE_AA);

    // second filter
    //cvtColor(tframe, frame_gray, cv::COLOR_BGR2GRAY);
    //frame_gray = frame_gray > 128; 
    //bitwise_not(frame_gray, frame_bw);
    //findSquares(frame_bw, squares2);
//polylines(frame, squares2, true, Scalar(255, 255, 0), 3, LINE_AA);
// filter
//    vector<vector<Point>> blackRectangles;
//
//    for (const auto& contour : contours) {
//        Rect rect = boundingRect(contour);
//        Mat roi = frame(rect);
//        Scalar meanColor = mean(roi);
//    
//        // Vérifiez si la couleur moyenne est proche du noir (par exemple, RVB (0, 0, 0)).
//        if (meanColor[0] < 10 && meanColor[1] < 10 && meanColor[2] < 10) {
//            blackRectangles.push_back(contour);
//        }
//    }

//    for (const auto& contour : blackRectangles) {
//        drawContours(frame, vector<vector<Point>>{contour}, -1, Scalar(0, 255, 0), 2);
//    }

    for (auto& square : squares) {
        // Calculez le centre du contour
        cv::Moments M = cv::moments(square);
        cv::Point contour_center(M.m10 / M.m00, M.m01 / M.m00);

        // Calculez la différence entre le centre du contour et le centre de l'image
        cv::Point relative_position = contour_center - image_center;

        // Affichez la position relative
        std::cout << "Position relative du contour : x = " << relative_position.x << ", y = " << relative_position.y << std::endl;

        // Vous pouvez également dessiner un cercle au centre du contour pour le visualiser
        cv::circle(frame, contour_center, 5, cv::Scalar(0, 0, 255), -1);
    }
    cv::circle(frame, image_center, 5, cv::Scalar(255, 0, 0), -1);
    cout << "\n";
    // show image
    imshow("Live", frame);
    if (waitKey(5) >= 0)
    break;
 }
 
 // the camera will be deinitialized automatically in VideoCapture destructor
 return 0;
}
