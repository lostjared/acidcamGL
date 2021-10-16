#include"ac.h"
#include<cstdlib>
#include<ctime>

extern "C" void filter(cv::Mat  &frame) {
    cv::Mat copy = frame.clone();
    ac::DetectEdges(frame);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b &cpixel = copy.at<cv::Vec3b>(z, i);
            if(pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0) {
                pixel[0] += cpixel[0];
                pixel[1] += cpixel[1];
                pixel[2] += cpixel[2];
            } else {
                pixel[0] = 255;
                pixel[1] = 255;
                pixel[2] = 255;
            }
        }
    }
}
