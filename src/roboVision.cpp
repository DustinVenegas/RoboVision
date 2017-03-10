// an app to detect and respond to hand gestures using OpenCV

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "RoboVision/Hand.h"

using namespace std;
using namespace cv;

// Global Variables(I know, too many):
Mat rawImage;
bool debugFlag = false;
const char* windowName = "RoboVision";
// TODO: pull out threshold values into config file.
int minY = 84;
int maxY = 230;
int minCr = 89;
int maxCr = 165;
int minCb = 104;
int maxCb = 119;
int blurSize = 3;
int elementSize = 2;

// Method Headers:
void parseFlags(int argc, char** argv);
Mat preProcess(Mat image);
vector<Point> getLargestContour(Mat mat);
size_t getLargestContourIndex(vector<vector<Point>> countours);
vector<Point> getFingerTips(vector<Point> contour, vector<Vec4i> defects, Rect bound);
float getDistance(Point p1, Point p2);
float getTriangleArea(Point p1, Point p2, float defectDepth);
float innerAngle (float px1, float py1, float px2, float py2, float cx1, float cy1);


// MAIN LOOP
int main(int argc, char** argv) {
    parseFlags(argc, argv);
   
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        printf("Camera not operational\n");
        return -1;
    }
    
    if (debugFlag) {
        namedWindow(windowName, WINDOW_AUTOSIZE);
    }

    for (;;) {
        cap >> rawImage;
        Mat processed = preProcess(rawImage);
        
        // Contour detection:
        vector<Point> handContour = getLargestContour(processed);

        // Convex hull:
        if (!handContour.empty()) {
            vector<vector<Point>> hull(1);
            convexHull(Mat(handContour), hull[0], false);
            
            if (debugFlag) {
                drawContours(rawImage, hull, 0, Scalar(0,255,0), 3);
            }

            // get Convex defects to detect fingertips:
            if (hull[0].size() > 2) {
                
                vector<int> hullIndexes;
                convexHull(Mat(handContour), hullIndexes, true);
                vector<Vec4i> convexDefects;
                cv::convexityDefects(Mat(handContour),
                                hullIndexes, convexDefects);
                
                Hand hand = Hand(handContour, 
                                     hullIndexes,
                                     convexDefects);

                Rect boundingBox = boundingRect(hull[0]);
                
                vector<Point> fingertips;
                if (debugFlag) {
                    // draw box around convex  
                    rectangle(rawImage, boundingBox, Scalar(255,0,0));
                    fingertips = hand.getFingertips(rawImage);
                } else {
                    fingertips = hand.getFingertips();
                }
                // draw circles around fingertips
                for (size_t i = 0; i < fingertips.size(); i++){
                    circle(rawImage, fingertips[i],
                           6, Scalar(0,255,0), 3);
                }
            }
        }
        
        if (debugFlag) {
            imshow(windowName, rawImage);
        }

        if (waitKey(30) == 27) {
            return 0;
        }
    }
}

void parseFlags(int argc, char** argv) {
    int c = 0;
    while ((c = getopt(argc, argv, "d")) != -1) {
        switch(c) {
            case 'd':
                debugFlag = true;
                break;
            default:
                //TODO: something, maybe? IDK.
                break;
        }
    }
}

Mat preProcess(Mat image) {
    Mat yccImage;
    
    // convert raw RGB image to YCrCb and apply thresholds:
    cvtColor(image, yccImage, CV_BGR2YCrCb);
    inRange(yccImage, Scalar(minY, minCr, minCb), 
            Scalar(maxY, maxCr, maxCb), yccImage);
    
    // apply blur to reduce noise:
    medianBlur(yccImage, yccImage, blurSize);

    //apply dilate to fill in gaps:
    Mat element = getStructuringElement(MORPH_ELLIPSE,
                    Size(2 * elementSize + 1, 2 * elementSize +1),
                    Point(elementSize, elementSize));
    dilate(yccImage, yccImage, element);
    return yccImage;
}

vector<Point> getLargestContour(Mat mat) {
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(mat, contours, hierarchy, CV_RETR_EXTERNAL,
                    CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        
    size_t largestContour = getLargestContourIndex(contours);       
       
    if (debugFlag) {
        drawContours(rawImage, contours, largestContour,
                     Scalar(0,0,255), 1);
    }
    if (!contours.empty()) { 
        return contours[largestContour];
    } else {
        vector<Point> empty;
        return empty;
    }
}

size_t getLargestContourIndex(vector<vector<Point>> contours) {
    size_t largestContour = 0;
    for (size_t i = 1; i < contours.size(); i++) {
        if (contourArea(contours[i]) > contourArea(contours[largestContour])) {
            largestContour = i;
        }
    }
    return largestContour;
}

vector<Point> getFingerTips(vector<Point> contour,
                            vector<Vec4i> defects, Rect bound) {
    vector<Point> validPoints; 
    Point center = Point(bound.x + bound.width/2,
                         bound.y + bound.height/2);
    Point indexFinger;
    Point thumb;
    float biggestDefectArea = 0;

    for (size_t i = 0; i < defects.size(); i++) {
        Point p1 = contour[defects[i][0]]; //start
        Point p2 = contour[defects[i][1]]; //end
        Point p3 = contour[defects[i][2]]; // pt furthest from convex edge
        float depth = defects[i][3]/256.0; // dist. from edge to p3
        
        // Draw defect lines:
        if (debugFlag) {
            line(rawImage, p1, p3, Scalar(255,0,0), 2);
            line(rawImage, p3, p2, Scalar(255,0,0), 2);
        }
        
        double inAngle = innerAngle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
        double length = getDistance(p1, p3);
        if (inAngle < 120 
            && depth > 0.1 * length
            && length > 0.2 * bound.height)
        {
            validPoints.push_back(p1);
            // the defect between the thumb and index should have
            // the most Area.
            float triangleArea = getTriangleArea(p1, p2, depth);
            if ( triangleArea > biggestDefectArea) {
                biggestDefectArea = triangleArea;
                if (getDistance(p1, p3) > getDistance(p2, p3)) {
                    indexFinger = p1;
                    thumb = p2;
                } else {
                    indexFinger = p2;
                    thumb = p1;
                    // draw circles around identified fingertips:
                    if (debugFlag) {
                        // thumb = black
                        circle(rawImage, thumb, 6, Scalar(0,0,0), 3);
                        // index = red
                        circle(rawImage, indexFinger, 6, Scalar(0, 0, 255), 3);
                    }
                }
            }
        }
    }
    
    // draw circles at  all the fingertips:
//     if (debugFlag) {
//         for (size_t i = 0; i < validPoints.size(); i++) {
//            circle(rawImage, validPoints[i], 9, Scalar(0,255,0), 2);
//        }
//    }

    return validPoints;
}

float getTriangleArea(Point p1, Point p2, float defectDepth) {
    float base = getDistance(p1, p2);   
    return (pow((base * defectDepth), 2)/2);
}

float getDistance(Point p1, Point p2) {
    float p1x = float(p1.x);
    float p1y = float(p1.y);
    float p2x = float(p2.x);
    float p2y = float(p2.y);
    float distance = sqrt( pow((p1x - p2x), 2) + pow((p1y - p2y), 2));
    return distance;
}


//TODO: refactor to use getDistance(). Make it look nicer.
float innerAngle (float px1, float py1, float px2, float py2,
                  float cx1, float cy1) {
    float dist1 = sqrt( (px1 - cx1)*(px1 - cx1) + (py1-cy1)*(py1 - cy1) );
    float dist2 = sqrt( (px2 - cx1)*(px2 - cx1) + (py2 - cy1)*(py2 - cy1) );

    float Ax, Ay;
    float Bx, By;
    float Cx, Cy;

    Cx = cx1;
    Cy = cy1;
    if (dist1 < dist2) {
        Bx = px1;
        By = py1;
        Ax = px2;
        Ay = py2;
    } else {
        Bx = px2;
        By = py2;
        Ax = px1;
        Ay = px1;
    }

    float Q1 = Cx - Ax;
    float Q2 = Cy - Ay;
    float P1 = Bx - Ax;
    float P2 = By - Ay;
    
    float A = acos((P1*Q1) / (sqrt(P1*P1 + P2*P2) * sqrt(Q1*Q1 + Q2*Q2) ) );

    A = A*180/CV_PI;

    return A;
}
