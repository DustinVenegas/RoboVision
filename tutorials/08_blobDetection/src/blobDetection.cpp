#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat image, normalizedImage;
char* windowName = "Blob Detection Demo";
int minConvexity = 0.00;
int maxConvexity = 87;
int colorThreshold = 100;
int minInertia = 0;
int maxInertia = 50;
int minArea = 1250;
int maxArea = 50000;

SimpleBlobDetector::Params params;
Ptr<SimpleBlobDetector> detector;

void setMinConvexityThreshold(int, void*);
void setMaxConvexityThreshold(int, void*);
void setColorThreshold(int, void*);
void setMinInertia(int, void*);
void setMaxInertia(int, void*);
void setMinArea(int, void*);
void setMaxArea(int, void*);


int main(int argc, char** argv) {
    VideoCapture cap(0);
    
    namedWindow(windowName, WINDOW_AUTOSIZE);
    
    // trackbar for the minimum blob convexity 
    createTrackbar("minConvexity", windowName, &minConvexity, maxConvexity, setMinConvexityThreshold );
    
    // trackbar for max blob convexity
    createTrackbar("Max Convexity", windowName, &maxConvexity, maxConvexity, setMaxConvexityThreshold );

    // trackbar for Color THreshold:
    createTrackbar("Color Threshold", windowName, &colorThreshold, 255, setColorThreshold );

    // trackbar for the Inertia Ratio:
    createTrackbar("Min Inertia Ratio", windowName, &minInertia, 100, setMinInertia );

    // trackbar for Max Inertia
    createTrackbar("max Inertia Ratio", windowName, &maxInertia, 100, setMaxInertia );

    // trackbar for Min Area
    createTrackbar("Min Blob Area", windowName, &minArea, 10000, setMinArea );


    // trackbar for MaxArea
    createTrackbar("Max Blob Area", windowName, &maxArea, 270000, setMaxArea );

    // set default params for detector:
    params.filterByConvexity = true;
    params.minConvexity = 0.00;
    
    params.filterByColor = false;
    params.blobColor = colorThreshold;

    params.filterByInertia = true;
    params.maxInertiaRatio = 1.0;
    params.minInertiaRatio = 0.5;

    params.filterByArea = true;
    params.minArea = minArea;
    params.maxArea = maxArea;

    detector = SimpleBlobDetector::create(params);

    for (;;) {
        cap >> image;
        
        //normalizedImage.create(image.size(), image.type());
        //normalize(image, normalizedImage, 1, 125); 

        //detect blobs
        vector<KeyPoint> keypoints;
        //detector->detect(normalizedImage, keypoints);
        detector->detect(image, keypoints);

        // draw red circles around blobs
        Mat new_image;
        drawKeypoints(image, keypoints, new_image, Scalar(0,0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

        imshow(windowName, new_image);

        if (waitKey(10) == 27) {
            return 0;
        }
    }
}

void setMinConvexityThreshold(int, void*) {
    float setting = (float)minConvexity/(float)100;
    printf("setting minimum convexity to %f\n", setting); 
    params.minConvexity = setting;
    detector = SimpleBlobDetector::create(params);
}

void setMaxConvexityThreshold(int, void*) {
    float setting = (float)maxConvexity/(float)100;
    printf("setting maximum convexity to %f\n", setting); 
    params.minConvexity = setting;
    detector = SimpleBlobDetector::create(params);
}
void setColorThreshold(int, void*) {
    int setting = colorThreshold;
    params.blobColor = setting;
    printf("setting Color Value Threshold to %d\n", setting);
    detector = SimpleBlobDetector::create(params);
}

void setMinInertia(int, void*) {
    float setting = (float)minInertia/(float)100 ;
    params.minInertiaRatio = setting;
    printf("setting Min Inertia Ratio to %f\n", setting);
    detector = SimpleBlobDetector::create(params);
}

void setMaxInertia(int, void*) {
    float setting = (float)maxInertia/(float)100 ;
    params.maxInertiaRatio = setting;
    printf("setting Max Inertia Ratio to %f\n", setting);
    detector = SimpleBlobDetector::create(params);
}

void setMaxArea(int, void*) {
    params.maxArea = maxArea;
    printf("setting Max Area to %d\n", maxArea);
    detector = SimpleBlobDetector::create(params);
}

void setMinArea(int, void*) {
    params.minArea = minArea;
    printf("setting Min Area to %d\n", minArea);
    detector = SimpleBlobDetector::create(params);
}
