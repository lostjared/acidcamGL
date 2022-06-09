#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int SIZE_X = 16;
    static constexpr int SIZE_Y = 16;
    double blend = 1.0;
    static double max_inc = 5.0;
    double inc = max_inc/((SIZE_X*frame.cols) + (SIZE_Y*frame.rows));
    for(int z = 0; z < frame.rows; z += SIZE_Y) {
        for(int i = 0; i < frame.cols; i += SIZE_X) {
            for(int x = 0; x < SIZE_X && x+i < frame.cols; x++) {
                for(int y = 0; y < SIZE_Y && y+z < frame.rows; y++) {
                    cv::Vec3b &pixel = ac::pixelAt(frame, z+y, i+x);
                    pixel[0] = ac::wrap_cast(blend * pixel[0]);
                    pixel[1] = ac::wrap_cast(blend * pixel[1]);
                    pixel[2] = ac::wrap_cast(blend * pixel[2]);
                }
            }
            blend += inc;
        }
    }
    
    static int dir = 1;
    if(dir == 1) {
        max_inc += 1.0;
        if(max_inc > 25) {
            max_inc = 25;
            dir = 0;
        }
    } else {
        max_inc -= 1.0;
        if(max_inc <= 1.0) {
            max_inc = 1.0;
            dir = 1;
        }
    }
}
