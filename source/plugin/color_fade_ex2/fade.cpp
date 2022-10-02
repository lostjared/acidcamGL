#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static int lazy = 1;
    if(lazy == 1) {
        lazy = 0;
        srand(static_cast<unsigned int>(time(0)));
    }
    
    static cv::Scalar color(rand()%255, rand()%255, rand()%255);
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            for(int q = 0; q < 3; ++q) {
                pixel[q] = ac::wrap_cast((pixel[q]+color[q]));
               
            }
        }
    }
    
    for(int q = 0; q < 3; ++q) {
        color[q] += 1;
        if(color[q] >= 254) {
            color[q] = rand()%255;
        }
    }
}
