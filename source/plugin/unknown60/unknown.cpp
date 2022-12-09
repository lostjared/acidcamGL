#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    
    static double num = 1.0;
    static int dir = 1;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            
            for(int q = 0; q < 3; ++q)
                pixel[q] ^= ac::wrap_cast(num * pixel[q]);
            
        }
        
        if(dir == 1) {
            num += 0.0001;
            if(num > 5)
                dir = 0;
        } else {
            num -= 0.0001;
            if(num <= 1)
                dir = 1;
        }
    }
    
}
