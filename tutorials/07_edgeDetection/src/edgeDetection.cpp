// test script for Canny Edge Detection

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

// Global Variables:
Mat image, image_gray, processed, edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* windowName = "Edge Detection";

void cannyThreshold(int, void*);

void smoothImage();

int main(int argc, char** argv) {
    VideoCapture capture(0);
    if (!capture.isOpened()) {
        printf("Camera not operational");
        return -1;
    }

    namedWindow(windowName, WINDOW_AUTOSIZE);
    createTrackbar( "Min Threshold:", windowName, &lowThreshold, max_lowThreshold, cannyThreshold);

    for (;;) {
        capture >> image;
        
        // create a matrix of same size & type as image
        processed.create (image.size(), image.type());
        
        cvtColor(image, image_gray, CV_BGR2GRAY);

        cannyThreshold(0,0);

        if (waitKey(10) == 27) {
            break;
        }
    }
}

void cannyThreshold(int, void*) {
    //reduce noise with a kernal 3x3
    blur ( image_gray, edges, Size(3,3) );

    // Canny detector
    Canny( edges, edges, lowThreshold, lowThreshold*ratio, kernel_size);
    
    //use canny's output as a mask
    processed = Scalar::all(0);

    image.copyTo(processed, edges);
    imshow(windowName, processed);
}
