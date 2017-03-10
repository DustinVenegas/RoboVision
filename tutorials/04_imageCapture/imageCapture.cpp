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

using namespace cv;
using namespace std;

int fileIndex = 0;


int getLastImageIndex(string directory) {
    DIR *dir ;
    struct dirent *entry;
    int newIndex = 0;
    if ((dir = opendir(directory.c_str())) != NULL) {
             
        while (entry = readdir(dir)) {
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
        printf("final index: %d\n", newIndex);
        printf("closing dir\n");
        closedir(dir);
        printf("dir closed. returning newIndex.");
        return newIndex;
    } 
    else {
        printf("getLastImageIndex(): cannot open directory %s", directory.c_str());
        return -1;
    }
}

void saveImage(string dir, Mat image) {
    stringstream ss;
    ss << dir << "/" << fileIndex << ".jpg";
    imwrite (ss.str(), image);
    printf("saving file: %d\n", fileIndex);
    fileIndex +=1;
}

int main(int argc, char** argv) {
    string folder;
    
    // if user didn't specify a classifier name, exit:
    if( argc !=2 ){
        printf("No Object name specified.\n");
        printf("Usage: imageCapture <name for image>");
        return -1;
    } else {
        folder = string(argv[1]);
    }
   
    //create a new folder with the argument name:
    if ( mkdir(folder.c_str(), S_IRWXU) == -1) {
        if (errno == EEXIST) {
            // dir already exists, get the index of the last image:
            int newIndex = getLastImageIndex(folder);
            printf("folder already exists. Starting current index at %d\n", newIndex);
            fileIndex = newIndex;           
        } else {
            printf("Error creating folder: %s",  folder.c_str());
            return -1;
        }
    } else {
        cout << "Created folder: " << folder << endl;
    }

    //open webcam
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "camera not operational" << endl;
        return -1;
    }

    for (;;) {
        Mat cameraFrame;
        cap >> cameraFrame;
        imshow("Camera Vision", cameraFrame);
        
        saveImage(folder, cameraFrame);
        //break by pressing 'Esc':
        if (waitKey(10) == 27) break;
        
        sleep(1);
    }
    return 0;
}


