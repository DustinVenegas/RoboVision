#include <opencv2/opencv.hpp>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include "RoboVision/CategoryTester.h"
#include "RoboVision/HandFeatureExtractor.h"

using namespace cv;
using namespace std;

CategoryTester::CategoryTester() {
	_posSamples = 0;
	_negSamples = 0;
	_correct = 0;
	_incorrect = 0;

}

void CategoryTester::test(char* positives) {
	testPath(positives, true);
	printResults();
}

void CategoryTester::test(char* positives, char* negatives) {
	testPath(positives, true);
	testPath(negatives, false);
	printResults();
}

// boolean 'match' tells the method if it's positive samples or 
// negatvie samples
void CategoryTester::testPath(char* path, bool match) {
	int step = isFile(path);
    switch (step) {
        case  1: {
            printf("Argument is a file. Testing . . . \n");
            if (match) {
            	_posSamples +=1;
            } else {
            	_negSamples +=1;
            };
            bool matched = testImageFile(path);
            if ((matched & match) | (!matched & !match)) {
            	_correct++;
            } else {
            	_incorrect++;
            }
        }
        break; 
        case 0: {
            printf("Argument is a directory. Iterating . . .\n");
            vector<string> fileList = getFileNames(path);
            if (match) {
                _posSamples = fileList.size();
            } else {
                _negSamples = fileList.size();
            }
            for(unsigned int i = 0; i < fileList.size(); i++) {
                string fileStr = fileList[i];
                bool matched = testImageFile(fileStr);
                if ((matched & match) || (!matched & !match)) {
	            	_correct++;
	            } else {
	            	_incorrect++;
	            }
            }
        }
        break;
        default: {
            printf("invalid filepath\n");
        }
        break;
    }
}

int CategoryTester::isFile( char* name) {
    // fprintf(stdout, "isFile:: checking path: %s . . .\n", name);
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

vector<string> CategoryTester::getFileNames(char* dirPath) {
    printf("opening dir . . .\n");
    vector<string> fileNames;
    DIR *dir = opendir(dirPath);
    struct dirent *ent;
    int index = strlen(dirPath);
    // add file separator if it wasn't passed.
    if (dirPath[index-1] != '/') {
        strcat(dirPath, "/");
    }
    while ((ent = readdir (dir)) != NULL) {
        char* fileName = ent->d_name;
        // +1 because of '\0' at the end
        char* fileStr = (char*) malloc(strlen(dirPath) + 1);
        strcpy(fileStr, dirPath);
        strcat(fileStr, fileName);
        if ((isFile(fileStr) == 1) && (fileName[0] != '.')) {
            fileNames.push_back(fileStr);
        } else {
            //DEBUGGING
            fprintf(stdout, "getFileNames:: %s is not a regular file, skipping\n", fileStr);
        }
    }
    closedir(dir);
    return fileNames;
}
bool CategoryTester::testImageFile(string file) {
	Mat image = imread(file, CV_LOAD_IMAGE_COLOR);
    HandFeatureExtractor handFeatExt = HandFeatureExtractor();
    bool matched = false;
    printf("Testing file: %s\n", file.c_str());
    if (handFeatExt.detect(image)) {
    	printf("POSITIVE\n");
    	matched = true;
    } else {
    	printf("NEGATIVE\n");
    }
    return matched;
}

void CategoryTester::printResults() {
    printf("---INPUTS---\n");
	printf("number of positive samples: %d\n", _posSamples);
	printf("number of negative samples: %d\n", _negSamples);
    printf("---OUPUT---\n");
    printf("number of correct guesses: %d\n", _correct);
    printf("number of incorrect guesses: %d\n", _incorrect);
    float percentage = getPercentage(_correct, _incorrect);
    printf("Probability of correct guess: %f\n", percentage);
}

float CategoryTester::getPercentage(int operand, int dividend) {
    float ans = (float)operand / (float)dividend;
    return ans;
}
