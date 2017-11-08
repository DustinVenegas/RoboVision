// Histogram of Oriented Gradients Feature Extractor
#include <opencv2/opencv.hpp>
#include "RoboVision/HOGFeature.h"

using namespace cv;

HOGFeature::HOGFeature() {

}

Mat HOGFeature::extract(Mat mat) {
    preProcess(mat);


}

void HOGFeature::preProcess(Mat mat) {
    //TODO: resize image to standard dimensions
}
