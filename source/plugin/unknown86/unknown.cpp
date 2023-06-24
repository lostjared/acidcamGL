// color xor (variable random inc/dec)
#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static int lazy = 0;
    static double alpha[3] = {
        static_cast<double>(rand()%4),
        static_cast<double>(rand()%4),
        static_cast<double>(rand()%4)
    };
  
    if(lazy == 0) {
        srand(static_cast<unsigned int>(time(0)));
        alpha[0] = static_cast<double>(rand()%4);
        alpha[1] = static_cast<double>(rand()%4);
        alpha[2] = static_cast<double>(rand()%4);
        lazy = 1;
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
    static int dir[3] = {rand()%2, rand()%2, rand()%2};
    
    static double max_x[3]= {1.0, 2.0, 3.0};
    static double min_x[3] = {3.0, 2.0, 1.0};
    
    for(int q = 0; q < 3; ++q) {
        double r = static_cast<double>(rand()%10);
        if(dir[q] == 1) {
            alpha[q] += 0.005*r;
            if(alpha[q] >= max_x[q]) {
                alpha[q] = max_x[q];
                dir[q] = rand()%2;
                double r2 = static_cast<double>(rand()%10);
                double rv = 0.01*r2;
                max_x[q] += rv;
                if(max_x[q] >= 3.0)
                    max_x[q] = 3.0;
            }
        } else {
            alpha[q] -= 0.005*r;
            if(alpha[q] <= min_x[q]) {
                alpha[q] = min_x[q];
                dir[q] = rand()%2;
                double r2 = static_cast<double>(rand()%10);
                double rv = 0.01*r2;
                min_x[q] -= rv;
                if(min_x[q] <= 1)
                    min_x[q] = 1;
            }
        }
    }
}
