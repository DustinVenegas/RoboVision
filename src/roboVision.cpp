// an app to detect and respond to hand gestures using OpenCV

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "RoboVision/Hand.h"
#include "RoboVision/HandFeatureExtractor.h"

using namespace std;
using namespace cv;

// Global Variables(I know, too many):
Mat rawImage;
HandFeatureExtractor handFeatExt;
bool debugFlag = false;
bool testFlag = false;
const char* windowName = "RoboVision";

// Method Headers:
void parseFlags(int argc, char** argv);
void testloop(char** filePath);
// MAIN LOOP
int main(int argc, char** argv) {
    parseFlags(argc, argv);
    namedWindow(windowName, WINDOW_AUTOSIZE);
    
    //flag-specific setup
    if (debugFlag) {   
        VideoCapture cap(0);

        if (!cap.isOpened()) {
            printf("Camera not operational\n");
            return -1;
        }
        printf("Camera operational\n");
    }

    //main loop
    for (;;) {
        cap >> rawImage;
        handFeatExt = HandFeatureExtractor();
        if (handFeatExt.detect(rawImage)) {
            // TODO: something useful?
            printf("Found a hand!");
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
            case 't':
                testFlag = true;
                break
            default:
                //TODO: something, maybe? IDK.
                break;
        }
    }
}

void testLoop(char** filePath) {
    Mat image = imread(filepath, 1);
     handFeatExt = HandFeatureExtractor();
    if (handFeatExt.detect(rawImage)) {
        // TODO: something useful?
        printf("Found a hand!");
    }
}

