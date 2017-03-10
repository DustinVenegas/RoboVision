// Now that I have a method for gathering example images, I need a way
// to clean up the raw data and break it out into three buckets:
// Training Data, for the classifier 60%
// Dev Test Data, for use when developing classifer: 20%
// QA Test Data, for use on the finished product: 20%

#include <opencv2/opencv.hpp>
#include <dirent.h>// iterate through files
#include <sys/stat.h>// for mkdir
#include <iostream>

using namespace cv;
using namespace std;

int fileIndex = 0;
bool isDev = false;
vector<string> nameArray;

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
    nameArray.push(ss.string();
    fileIndex +=1;
}

void saveIndexFile(string dir) {
    stringstream ss;
    // TODO: use nameArray to write an index file for the 
    // Training directory. That file gets used by the classifier.
}

int main(int argc, char** argv) {
    string inFolder;
    string outFolder;
    string trainFolder;
    string devFolder;
    string qaFolder;
    
    // no folder specified
    if (argc !=2) {
        printf("No folder name specified.\n");
        printf("Usage: imageTrainer <raw_data_folder>\n");
        return -1;
    } else { 
        inFolder = string(argv[1]);
        outFolder = inFolder + "_processed";   
        trainFolder = outFolder + "/training_data";
        devFolder = outFolder + "/dev_data";
        qaFolder = outFolder + "/qa_data";
    }
    
    //create a new folder with the argument name:
    if ( mkdir(outFolder.c_str(), S_IRWXU) == -1) {
        if (errno == EEXIST) {
            // dir already exists:
            printf("ERROR: output directory already exists\n");
            return -1;
        } else {
            printf("Error creating folder: %s",  outFolder.c_str());
            return -1;
        }
    } else {
        mkdir(trainFolder.c_str(), S_IRWXU);
        mkdir(devFolder.c_str(), S_IRWXU);
        mkdir(qaFolder.c_str(), S_IRWXU);

        cout << "Created folder: " << outFolder << endl;
    }
    
    // iterate through directory and pull in image files:
    DIR *dir;
    struct dirent *entry;
    if((dir = opendir(inFolder.c_str()))) {
        while (entry = readdir(dir)) {
            string imageFile = inFolder + "/" + string(entry->d_name);
            Mat image = imread(imageFile);
            
            // save the image according to index number:
            // to save 60% of images in trainFolder, we use indexes divisible by 2 or 5:
            if ( (fileIndex % 2 == 0) || (fileIndex % 5 == 0) ) {
                saveImage(trainFolder, image);    
            } else { // split the remaining 40% among dev and QA test folders:
                if (isDev) {
                    saveImage(devFolder, image);
                    isDev = false;
                } else {
                    saveImage(qaFolder, image);
                    isDev = true;
                }
            }
        }
    }
}

