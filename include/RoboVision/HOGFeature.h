#include <opencv2/opencv.hpp>

#ifndef HOGFEATURE_h
#define HOGFEATURE_h

using namespace cv;

class HOGFeature {
    public:
        HOGFeature;
        extract(Mat mat);

    private:
        void preProcess(Mat mat);

}

#endif
