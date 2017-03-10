// grabs a frame from a connected webcam and displays it.
// stolen from thefreecoder.wordpress.com/2012/09/11/opencv-c-video-capture/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
 
int main(int argc, char** argv) {
    // VideoCapture takes and int argument for the index
    // of cameras attached to the machine:
    VideoCapture cap(0);
    // check if camera object has been initialized:
    if (!cap.isOpened()) {
        cout << "camera not operational" << endl;
        return -1;
    }
    for (;;) {
        Mat cameraFrame;
        cap >> cameraFrame; // get a new frame from camera
        imshow("Camera Output", cameraFrame);
    if (waitKey(10) == 27) break; //stop the capture by pressing ESC
    }
    //the camera gets closed automatically on exit
    return 0;
}
