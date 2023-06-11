// strobe
#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static double alpha = 1.0;
    static int col = 0;
    for(int z = 0; z < frame.rows; z++) {
        for(int i = 0; i < frame.cols; i++) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            auto value = ac::wrap_cast(alpha * pixel[col]);
            for(int q = 0; q < 3; ++q)
                pixel[q] = pixel[q]^value;
           
        }
    }
    alpha += 0.01;
    if(alpha >= 3.0)
        alpha = 1.0;
    ++col;
    if(col > 2)
        col = 0;
}
