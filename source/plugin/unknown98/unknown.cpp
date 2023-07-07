// color xor alpha 0.5 (rand index by i)
#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    
    static auto drawSquare = [](cv::Mat &f, double alpha[3], int x, int y, int sx, int sy) {
        
        int w = 1+(rand()%sx);
        int h = 1+(rand()%sy);
        for(int i = x; i < x+w && i < f.cols; ++i) {
            for(int z = y; z < z+h && z < f.rows; ++z) {
                cv::Vec3b &pixel = ac::pixelAt(f, z, i);
                unsigned char value[3];
                for(int q = 0; q < 3; ++q) {
                    value[q] = ac::wrap_cast(alpha[q] * pixel[q]);
                    pixel[q] = pixel[q]^value[q];
                }
            }
        }
    };
    
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
        inc[0] = 0.03 * r[0];
        inc[1] = 0.03 * r[1];
        inc[2] = 0.03 * r[2];
    }
    
    
    int rand_square = 10+rand()%75;
    for(int r = 0; r < rand_square; ++r) {
        int xx = rand()%frame.cols;
        int yy = rand()%frame.rows;
        int sx = 1+(rand()%frame.cols);
        int sy = 1+(rand()%frame.rows);
        drawSquare(frame, alpha, xx, yy, sx, sy);
    }
    
    static int q = 0;
    static double increase_i = 0.1;
    
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
    increase_i += 0.05;
    if(increase_i > 0.5) {
        increase_i = 0.1;
        q = rand()%3;
    }
}
