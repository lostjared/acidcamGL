#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    for(int z = 0; z < frame.rows; z += 2) {
        int col_start = frame.cols/4+rand()%50;
        int col_space = rand()%frame.cols/2;
        for(int i = col_start; i < col_start+col_space; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            pixel[0] ^= 150+rand()%50;
            pixel[1] ^= 150+rand()%50;
            pixel[2] ^= 150+rand()%50;
        }
    }
}
