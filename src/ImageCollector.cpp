// opens a webcam and saves images for object detection.
// to keep things organized, it creates a folder in the 
// local directory, then numbers the images.
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <regex>
#include "RoboVision/ImageCollector.h"

using namespace cv;
using namespace std;

void mouseCallbackWrapper(int one, int two, int three, int four, void* ptr) {
    ImageCollector* ic = reinterpret_cast<ImageCollector*> (ptr);
    ic->mouseCallback(one, two, three, four);
}

ImageCollector::ImageCollector() {
    _fileIndex = 0;
    _pt1.x = 0;
    _pt1.y = 0;
    _pt2.x = 0;
    _pt2.y = 0;
    _drawBound = false;
    _subFrameBox = false;
    _currentFrame = Mat();
    _WINDOW_NAME = "RoboVision Image Collector";
}

int ImageCollector::collectorLoop(string folderName){
    
    createDir(folderName);

    //open webcam
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "camera not operational" << endl;
        return -1;
    } else {
        printf("Camera operational\n");
    }

    //create window
    namedWindow(_WINDOW_NAME, 1);

    //set callback
    cv::setMouseCallback(_WINDOW_NAME, mouseCallbackWrapper,
                         (void *) this);

    const Scalar GREEN = Scalar(0,255,0);

    for (;;) {
        cap >> _currentFrame;
        // imshow("RoboVision Image Collector", _currentFrame);
        redraw();

        switch(waitKey(30)) {
            // save image by pressing spacebar
            case 32: {
                // if a box was drawn
                if ((_subFrameBox) ) {
                    Mat subFrame = crop();
                    saveImage(folderName, subFrame);
                } else {
                    saveImage(folderName, _currentFrame);
                }
                break;
            }
            // break the loop by pressing 'Esc'
            case 27:
                break;
        }   
    }
    return 0;
}

bool ImageCollector::createDir(string dirName) {
    // if user didn't specify a classifier name, exit:
    if(dirName == ""){
        printf("No Object name specified.\n");
        printf("Usage: imageCapture <name for image>");
        return false;
    }
   
    //create a new folder with the argument name:
    if ( mkdir(dirName.c_str(), S_IRWXU) == -1) {
        if (errno == EEXIST) {
            // dir already exists, get the index of the last image:
            int newIndex = getLastImageIndex(dirName);
            printf("folder already exists. Starting current index at %d\n", newIndex);
            _fileIndex = newIndex;
            return true;         
        } else {
            printf("Error creating folder: %s",  dirName.c_str());
            return false;
        }
    } else {
        printf("Created folder: %s\n",  dirName.c_str());
        return true;
    }
}

int ImageCollector::getLastImageIndex(string directory) {
    DIR *dir ;
    struct dirent *entry;
    int newIndex = 0;
    if ((dir = opendir(directory.c_str())) != NULL) {
             
        while ((entry = readdir(dir))) {
            char* name = entry->d_name;
            string s = name;
            printf("found file: %s\n", name);
            
            // extract int from filename string:
            int number = atoi(s.substr(0,-3).c_str());
            printf("file number: %d\n", number);

            if (number > newIndex) {
                newIndex = number;
            }
        }
        // printf("final index: %d\n", newIndex);
        // printf("closing dir\n");
        closedir(dir);
        newIndex += 1;
        return newIndex;
    } 
    else {
        printf("getLastImageIndex(): cannot open directory %s", directory.c_str());
        return -1;
    }
}

void ImageCollector::saveImage(string dir, Mat image) {
    stringstream ss;
    ss << dir << "/" << _fileIndex << ".bmp";
    imwrite(ss.str(), image);
    printf("saving file: %d\n", _fileIndex);
    _fileIndex +=1;
}

void ImageCollector::mouseCallback(int event, int x, int y, 
                   int flags) {
    if (event == EVENT_LBUTTONDOWN) {
        // first click starts a bounding box, resets one if exists
        if (!_drawBound) {
            _pt1.x = x;
            _pt1.y = y;
            // setting a small difference so that rectangle doesn't get weird.
            _pt2.x = x + 1;
            _pt2.y = y + 1;
            printf("point 1:\n");
            printf("   x = %d\n", _pt1.x);
            printf("   y = %d\n", _pt1.y);
            _drawBound = true;
        } else { // second click completes drawing bound
            _pt2.x = x;
            _pt2.y = y;
            printf("point 2\n");
            printf("   x = %d\n", _pt2.x);
            printf("   y = %d\n", _pt2.y);
            _drawBound = false;
            _subFrameBox = true;
        }

    } else if (event == EVENT_RBUTTONDOWN) {
        printf("rightbutton click\n");
        // clears bounding box if one exists
        _pt1.x = 0;
        _pt1.y = 0;
        _pt2.x = 1;
        _pt2.y = 1;
        _drawBound = false;
        _subFrameBox = false;
    } else if (event == EVENT_MOUSEMOVE) {
        // if boundingbox in progress
        // redraw box with new posish
        if (_drawBound) {
            _pt2.x = x;
            _pt2.y = y;
        }
    }
    redraw();
}

void ImageCollector::redraw() {
    if (_subFrameBox) {
        const Scalar GREEN = Scalar(0,255,0);
        // draw the rectangle
        rectangle(_currentFrame, _pt1, _pt2, GREEN);
    }
    imshow(_WINDOW_NAME, _currentFrame);
}

Mat ImageCollector::crop() {
    // rectangle overlaps the top and left sides of the Mat,
    // so I gotta trim those sides:
    if (_pt1.x < _pt2.x) {
        if (_pt1.y < _pt2.y) {
            _pt1.x +=1;
            _pt1.y +=1;
        } else {
            _pt1.x +=1;
            _pt2.y +=1;
        }
    } else {
        if (_pt1.y < _pt2.y) {
            _pt1.y +=1;
            _pt2.x +=1;
        } else {
            _pt2.y +=1;
            _pt2.x +=1;
        }
    }
    Rect_<int> roi = Rect(_pt1, _pt2);
    Mat output = Mat(_currentFrame, roi);
    return output;
}
