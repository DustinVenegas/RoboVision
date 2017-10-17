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
const char* windowName = "RoboVision";

// Method Headers:
void parseFlags(int argc, char** argv);

// MAIN LOOP
int main(int argc, char** argv) {
    parseFlags(argc, argv);
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        printf("Camera not operational\n");
        return -1;
    }
    printf("Camera operational\n");

    if (debugFlag) {
        namedWindow(windowName, WINDOW_AUTOSIZE);
    }

    for (;;) {
        cap >> rawImage;
        handFeatExt = HandFeatureExtractor();
        handFeatExt.detect(rawImage);
                
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

