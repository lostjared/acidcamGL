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
    static int dir = 1;
    if(dir == 1) {
        alpha += 0.01;
        if(alpha >= 3.0) {
            alpha = 1.0;
            dir = 0;
        }
    } else {
        alpha -= 0.01;
        if(alpha <= 1) {
            alpha = 1.0;
            dir = 1;
        }
    }
}
