#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {

    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
        }
    }
}
