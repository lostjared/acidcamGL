// color xor increase
#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static double alpha = 1.0;
    for(int z = 0; z < frame.rows; z++) {
        for(int i = 0; i < frame.cols; i++) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            unsigned char value[3];
            for(int q = 0; q < 3; ++q) {
                value[q] = ac::wrap_cast(alpha * pixel[q]);
                pixel[q] = pixel[q]^value[q];
            }
           
        }
    }
    alpha += 0.001;
    if(alpha >= 3.0)
        alpha = 1.0;
}
