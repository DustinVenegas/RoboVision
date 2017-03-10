#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;
using namespace cv;

// Here we experiment with converting an RGB image into an HSV and 
// messing with the thresholds

// HSV:
// H = Hue. The acutal color value, on a spectrum
// S = Saturation. How much color is there. Think bold vs pastel
// V = Value. How dark the color is. 0 = black.

// YCbCr:
// Y = 
// Cb = 
// Cr = 

bool hsvFlag = false;
bool yccFlag = false;

Mat image;
Mat processed;

char* inputWindow = "Original Image";
char* processedWindow = "Processed Image";

int blurSize = 3;
int elementSize = 2;
// default color space thresholds:
// HSV:
int minH = 0;
int maxH = 80;
int minS = 30;
int maxS = 255;
int minV = 106;
int maxV = 255;
//YCbCr:
int minY = 84;
int maxY = 230;
int minCr = 89;
int maxCr = 165;
int minCb = 90;
int maxCb = 119;

// METHOD HEADERS:
void setBlur(int, void*);
void printSettings();
void initHSV();
void initYCC();
Mat colorSpace(Mat mat);
void applyThresholds(Mat& mat);

// MAIN LOOP
int main(int argc, char** argv) {

    // parse CLI input:
    int c;
    while ((c = getopt(argc, argv, "hy")) != -1) {
        switch(c) {
            case 'h':
                //can't do HSV and YCbCr at the same time
                if (yccFlag) {
                    printf("can't do HSV and YCbCr at the same time");
                    return -1;
                } else {
                    printf("using HSV format.\n");
                    hsvFlag = true;
                    break;
                }
            case 'y':
                if (hsvFlag) {
                    printf("can't do HSV and YCbCr at the same time");
                    return -1;
                } else {
                    printf("Using YCbCr format.\n");
                    yccFlag = true;
                    break;
                }
            default:
                printf("Unrecognized argument. Please use the following:\n");
                printf("-h for HSV\n-y for YCbCr\n\n");
                abort();
        }
    }
        
    VideoCapture cap(0);

    namedWindow(inputWindow, WINDOW_AUTOSIZE);
    namedWindow(processedWindow, WINDOW_AUTOSIZE);
     
    // init proper trackbars:
    if (hsvFlag) {
        initHSV();
    } else if (yccFlag) {
        initYCC();
    } else {
        printf("no Color Space specified.\n");
        return -1;
    }

    createTrackbar("Blur Size", inputWindow, &blurSize, 39, setBlur );
    
    createTrackbar("Dilate Size", inputWindow, &elementSize, 20);

    for (;;) {
        cap >> image;
        
        processed = colorSpace(image);
        
        // apply thresholds from trackbars:
        applyThresholds(processed);

        // apply blur to smooth image.
        medianBlur(processed, processed, blurSize);
        
        // apply dilate to fill in gaps:
        Mat element = getStructuringElement(MORPH_ELLIPSE, 
                    Size(2 * elementSize + 1, 2 * elementSize + 1),
                    Point(elementSize, elementSize));
        dilate(processed, processed, element);

        imshow(inputWindow, image);
        imshow(processedWindow, processed);

        if (waitKey(10) == 27) {
            printSettings();
            return 0;
        }
    }
}
void initHSV() {
    createTrackbar("H Min", processedWindow, &minH, 180);
    createTrackbar("H Max", processedWindow, &maxH, 180);
    createTrackbar("S Min", processedWindow, &minS, 255);
    createTrackbar("S Max", processedWindow, &maxS, 255);
    createTrackbar("V Min", processedWindow, &minV, 255);
    createTrackbar("V Max", processedWindow, &maxV, 255);
}

void initYCC() {   
    createTrackbar("Y Min", processedWindow, &minY, 255);
    createTrackbar("Y Max", processedWindow, &maxY, 255);
    createTrackbar("Cr Min", processedWindow, &minCr, 255);
    createTrackbar("Cr Max", processedWindow, &maxCr, 255);
    createTrackbar("Cb Min", processedWindow, &minCb, 255);
    createTrackbar("Cb Max", processedWindow, &maxCb, 255);
}

Mat colorSpace(Mat mat) {
    Mat color;
     if (hsvFlag) {
            cvtColor(mat, color, CV_BGR2HSV);
        } else if (yccFlag) {
            cvtColor(mat, color, CV_BGR2YCrCb);
    }
    return color;
}

void applyThresholds(Mat& mat) {
    if (hsvFlag) {
         inRange(mat, Scalar(minH, minS, minV), 
                Scalar(maxH, maxS, maxV), mat);

    } else if (yccFlag) {
        inRange(mat, Scalar(minY, minCr, minCb),
                Scalar(maxY, maxCr, maxCb), mat);
    }
}

void setBlur(int, void*) {
    // MedianBlur creates an aperture of dimensions blurSize x blurSize
    // the aperture has to be > 1 and not divisible by 2.
    if (blurSize % 2 == 0 || blurSize == 0) {
        blurSize++;
        setTrackbarPos("Blur Size", inputWindow, blurSize);
    }
}

void printSettings() {
    printf("\n#### Final Settings ####\n");
    if (hsvFlag) {
        printf("H Min: %d\n", minH);
        printf("H Max: %d\n", maxH);
        printf("S Min: %d\n", minS);
        printf("S Max: %d\n", maxS);
        printf("V Min: %d\n", minV);
        printf("V Max: %d\n", maxV);
    } else if (yccFlag) {
        printf("Y Min: %d\n", minY);
        printf("Y Max: %d\n", maxY);
        printf("Cr Min: %d\n", minCr);
        printf("Cr Max: %d\n", maxCr);
        printf("Cb Min: %d\n", minCb);
        printf("Cb Max: %d\n", maxCb);
    }
    printf("Blur Size: %d\n", blurSize);
    printf("Element Size: %d\n", elementSize);
    printf("########################\n\n");
}
