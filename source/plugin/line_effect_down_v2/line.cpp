#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static double alpha = 1.0;
    for(int i = 0; i < frame.cols; ++i) {
        for(int z = 0; z < frame.rows; ++z) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            if((i%2)==0) {
                
            } else {
                pixel[0] = ac::wrap_cast(cosf(i));
                pixel[1] = ac::wrap_cast(sinf(z));
                pixel[2] = ac::wrap_cast(tanf(alpha));
                alpha += 0.01f;
            }
        }
    }
}
