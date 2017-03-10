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

bool isDev = true;
int fileIndex = 0;


int getLastImageIndex(string directory) {
    // search one of the smaller folders to take less time.
    string subFolder = directory + "/dev_data";
    DIR *dir ;
    struct dirent *entry;
    int newIndex = 0;
    if ((dir = opendir(subFolder.c_str())) != NULL) {
             
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
    fileIndex +=1;
}

int main(int argc, char** argv) {
    string folder;
    string trainFolder;
    string devFolder;
    string qaFolder;
    
    
    // if user didn't specify a classifier name, exit:
    if( argc !=2 ){
        printf("No Object name specified.\n");
        printf("Usage: imageCapture <name for image>");
        return -1;
    } else {
        folder = string(argv[1]);
        trainFolder = folder + "/training_data";
        devFolder = folder + "/dev_data";
        qaFolder = folder + "/qa_data";
    }
   
    //create a new folder with the argument name:
    if ( mkdir(folder.c_str(), S_IRWXU) == -1) {
        if (errno == EEXIST) {
            // dir already exists, get the index of the last image:
            int newIndex = getLastImageIndex(folder);
            printf("last index; %d", newIndex);
            return 0;
        } else {
            printf("Error creating folder: %s",  folder.c_str());
            return -1;
        }
    } else {
        mkdir(trainFolder.c_str(), S_IRWXU);
        mkdir(devFolder.c_str(), S_IRWXU);
        mkdir(qaFolder.c_str(), S_IRWXU);

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
        
        // save the image according to index number:
        // to save 60% of images in trainFolder, we use indexes divisible by 2 or 5:
        if ( (fileIndex % 2 == 0) || (fileIndex % 5 == 0) ) {
            saveImage(trainFolder, cameraFrame);
        } else { // split the remaining 40% among dev and QA test folders:
            if (isDev) {
                saveImage(devFolder, cameraFrame);
                isDev = false;
            } else {
                saveImage(qaFolder, cameraFrame);
                isDev = true;
            }
        }
        //break by pressing 'Esc':
        if (waitKey(10) == 27) break;
        
        sleep(1);
    }
    return 0;
}


