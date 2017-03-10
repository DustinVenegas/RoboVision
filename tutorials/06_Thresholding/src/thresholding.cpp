// Increases image contrast in preparation for edge detection.
// mostly taken from: http://docs.opencv.org/2.4/doc/tutorials/imgproc/threshold/threshold.html

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// Global Variables
int threshold_value = 0;
int threshold_type = 3;
const int max_value = 255;
const int max_type = 4;
int const max_BINARY_value = 255;

Mat image, image_gray, processed;
char* windowName = "Threshold Demo";

string trackbar_type = "Type: \n 0: Binary\n 1: Binary Inverted\n 2: Truncate\n 3: To Zero\n 4: To Zero Inverted";
string trackbar_value = "Value";

/// Function headers
void Threshold_Demo( int, void* );

int main(int argc, char** argv) {
    //open webcam
    VideoCapture capture(0); 
    if (!capture.isOpened()) {
        printf("camera not operational");
        return -1;
    }
    
    // window to display results
    namedWindow(windowName, WINDOW_AUTOSIZE);

    // trackbar for threshold type;
    createTrackbar( trackbar_type, windowName,
                    &threshold_type, max_type,
                    Threshold_Demo );
    createTrackbar( trackbar_value, windowName,
                    &threshold_value, max_value,
                    Threshold_Demo );
                    
    
    for (;;) {
        capture >> image;
        cvtColor(image, image_gray, CV_BGR2GRAY);
        Threshold_Demo(0, 0);

        if (waitKey(10) == 27) {
            break;
        }
    }
}

void Threshold_Demo( int, void* ){
    /* 0: Binary
    1: Binary Inverted
    2: Threshold Truncated
    3: Threshold to Zero
    4: Threshold to Zero Inverted
    */

    threshold( image_gray, processed, threshold_value, max_BINARY_value,threshold_type );

    imshow( windowName, processed );
}
