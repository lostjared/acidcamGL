// color xor (variable random increase)
#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static int lazy = 0;
    
    static double alpha[3] = {
        static_cast<double>(rand()%4),
        static_cast<double>(rand()%4),
        static_cast<double>(rand()%4)
    };
  
    static int dir[3] = {
        rand()%2,
        rand()%2,
        rand()%2
    };
    
    static double r[3] = {
        static_cast<double>(rand()%10),
        static_cast<double>(rand()%10),
        static_cast<double>(rand()%10)
    };
    
    static double inc[3] = {
        0.01 * r[0],
        0.01 * r[1],
        0.01 * r[2]
    };

    if(lazy == 0) {
        srand(static_cast<unsigned int>(time(0)));
        alpha[0] = static_cast<double>(rand()%4);
        alpha[1] = static_cast<double>(rand()%4);
        alpha[2] = static_cast<double>(rand()%4);
        lazy = 1;
        dir[0] = rand()%2;
        dir[1] = rand()%2;
        dir[2] = rand()%2;
        r[0] = static_cast<double>(rand()%10);
        r[1] = static_cast<double>(rand()%10);
        r[2] = static_cast<double>(rand()%10);
        inc[0] = 0.01 * r[0];
        inc[1] = 0.01 * r[1];
        inc[2] = 0.01 * r[2];
    }
    
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
    
  
    for(int q = 0; q < 3; ++q) {
        if(dir[q] == 1) {
            alpha[q] += inc[q];
            if(alpha[q] >= 3.0) {
                alpha[q] = 3.0;
                dir[q] = 0;
                inc[q] += 0.05;
                if(inc[q] > 1.5) {
                    r[q] = static_cast<double>(rand()%10);
                    inc[q] = 0.01 * r[q];
                }
            }
        } else {
            alpha[q] -= inc[q];
            if(alpha[q] <= 1.1) {
                alpha[q] = 1.1;
                dir[q] = 1;
                inc[q] += 0.05;
                if(inc[q] > 1.5) {
                    r[q] = static_cast<double>(rand()%10);
                    inc[q] = 0.01 * r[q];
                }
            }
        }
    }
}
