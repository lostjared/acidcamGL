#include "ac.h"

extern "C" void filter(cv::Mat &frame) {
    /*
    for(int z = 0; z < frame.rows; z += 32) {
        for(int i = 0; i < frame.cols; i += 32) {
            for(int y = 1; y < 31; ++y) {
                if(rand()%5 > 2) {
                    for(int x = 1; x < 31; ++x) {
                        cv::Vec3b &pixel = frame.at<cv::Vec3b>(z+y, i+x);
                        pixel = cv::Vec3b(0, 0, 0);
                    }
                }
            }
        }
    }
    */
}
