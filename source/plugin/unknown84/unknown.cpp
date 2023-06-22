// color xor (variable random inc/dec)
#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static int lazy = 0;
    static double alpha[3] = {
        static_cast<double>(rand()%4),
        static_cast<double>(rand()%4),
        static_cast<double>(rand()%4)
    };
    static int op_type[] = { rand()%3, rand()%3, rand()%3 };
  
    static auto op_func = [](int op_, unsigned char op1, unsigned char op2) -> unsigned char {
        switch(op_) {
            case 0:
                return op1 ^ op2;
                break;
            case 1:
                return op1 & op2;
                break;
            case 2:
                return op1 | op2;
                break;
        }
        return 0;
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
                pixel[q] = op_func(op_type[q], pixel[q], value[q]);
            }
            
        }
    }
    static int dir[3] = {rand()%2, rand()%2, rand()%2};
    
    static double max_x = 3.0, min_x = 1.0;
    
    for(int q = 0; q < 3; ++q) {
        double r = static_cast<double>(rand()%10);
        if(dir[q] == 1) {
            alpha[q] += 0.01*r;
            if(alpha[q] >= max_x) {
                alpha[q] = max_x;
                dir[q] = rand()%2;
                op_type[q] = rand()%3;
            }
        } else {
            alpha[q] -= 0.01*r;
            if(alpha[q] <= min_x) {
                alpha[q] = min_x;
                op_type[q] = rand()%3;
                dir[q] = rand()%2;
            }
        }
    }
}
