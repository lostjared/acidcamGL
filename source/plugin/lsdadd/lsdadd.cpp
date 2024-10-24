#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static int rgb[3] = { 1,2,3 };
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                for(int q = 0; q < 3; ++q) {
                    pixel[q] = pixel[q] * rgb[q];
                }
            }
    }
    rgb[0] += 1;
    rgb[1] += 2;
    rgb[2] += 3;
    static int counter = 0;
    ++counter;
    if(counter > 3) {
        counter = 0;
        rgb[0] = 1;
        rgb[1] = 2;
        rgb[2] = 3;
    }
}
