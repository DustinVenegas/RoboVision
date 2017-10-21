// an app to detect and respond to hand gestures using OpenCV

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "RoboVision/Hand.h"
#include "RoboVision/HandFeatureExtractor.h"

using namespace std;
using namespace cv;

// Global Variables(I know, too many):
Mat rawImage;
HandFeatureExtractor handFeatExt;
bool debugFlag = false;
bool testFlag = false;
char* _testPath = NULL;
const char* windowName = "RoboVision";

// Method Headers:
void parseFlags(int argc, char** argv);
void debugLoop();
void testLoop(char* filePath);
int isFile(const char* name);
// MAIN LOOP
int main(int argc, char** argv) {
    parseFlags(argc, argv);
    namedWindow(windowName, WINDOW_AUTOSIZE);
    
    //flag-specific setup
    if (debugFlag) {   
      debugLoop(); 
    } else if (testFlag) {
      testLoop(_testPath);
    }
}

void parseFlags(int argc, char** argv) {
    int c = 0;
    while ((c = getopt(argc, argv, "dt:")) != -1) {
        switch(c) {
            case 'd':
                debugFlag = true;
                break;
            case 't':
                testFlag = true;
                _testPath = optarg;
                break;
            case '?':
                if (optopt == 't') {
                    fprintf(stderr, "Unknown option: %s\n", optopt);
                }
            default:
                //TODO: something, maybe? IDK.
                break;
        }
    }
}

void debugLoop() {
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        printf("Camera not operational\n");
        exit (EXIT_FAILURE);
    }
    printf("Camera operational\n");
    
    for (;;) {    
        cap >> rawImage;
        handFeatExt = HandFeatureExtractor();
        if (handFeatExt.detect(rawImage)) {
            // TODO: something useful?
            printf("Found a hand!");
        }
                
        if (waitKey(30) == 27) {
            exit (0);
        }
    }
}

void testLoop(char* path) {
    int step = isFile(path);
    fprintf(stdout, "testing file: %s\n", path);
    switch (step) {
        case  1: {
            Mat image = imread(path, CV_LOAD_IMAGE_COLOR);
            handFeatExt = HandFeatureExtractor();
            if (handFeatExt.detect(image)) {
                // TODO: something useful?
                    printf("Found a hand!");
            }
        }
        break; 
        case 0: {
            DIR *dir = opendir(path);
            struct dirent *ent;
            while ((ent = readdir (dir)) != NULL) {
                char fileChar = ent->d_name[256];
                fprintf(stdout, "fileChar: %s\n", fileChar);
                // Mat image = imread(fileChar, CV_LOAD_IMAGE_COLOR);
                //handFeatExt = HandFeatureExtractor();
                //if (handFeatExt.detect(image)) {
                    // TODO: something useful?
                //    printf("Found a hand!");
                //}
            }
            closedir(dir);
        }
        break;
        default: {
            printf("invalid filepath");
        }
        break;
    }
}

int isFile(const char* name) {
    struct stat path_stat;
    stat(name, &path_stat);
    if (S_ISDIR(path_stat.st_mode)) {
        // folder:
        return 0;
    } else if (S_ISREG(path_stat.st_mode)) {
        // regular file:
        return 1;
    }
    return -1;
}

