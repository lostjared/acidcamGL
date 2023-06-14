// color xor
#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static double alpha[3] = {
        static_cast<double>(rand()%4),
        static_cast<double>(rand()%4),
        static_cast<double>(rand()%4)
    };
    for(int z = 0; z < frame.rows; z++) {
        for(int i = 0; i < frame.cols; i++) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            unsigned char value[3];
            for(int q = 0; q < 3; ++q) {
                value[q] = ac::wrap_cast(alpha[q] * pixel[q]);
                pixel[q] = pixel[q]^value[q];
            }
            
        }
    }
    static int dir[3] = {rand()%2, rand()%2, rand()%2};
    for(int q = 0; q < 3; ++q) {
        if(dir[q] == 1) {
            alpha[q] += 0.01;
            if(alpha[q] >= 3.0) {
                alpha[q] = 3.0;
                dir[q] = 0;
            }
        } else {
            alpha[q] -= 0.01;
            if(alpha[q] <= 1) {
                alpha[q] = 1.0;
                dir[q] = 1;
            }
        }
    }
}
