#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 10;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    collection.shiftFrames(frame);
    static int iter = 1;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            pixel = cv::Vec3b(0, 0, 0);
            for(int q = 0; q < iter; ++q) {
                cv::Vec3b pix;
                pix = ac::pixelAt(collection.frames[q], z, i);
                pixel[0] += ac::wrap_cast(1.0/iter * pix[0]);
                pixel[1] += ac::wrap_cast(1.0/iter * pix[1]);
                pixel[2] += ac::wrap_cast(1.0/iter * pix[2]);
            }
           
        }
    }
    static int dir = 1;
    if(dir == 1) {
        if(++iter > 9) {
            iter = 9;
            dir = 0;
        }
    } else {
        if(--iter <= 1) {
            iter = 1;
            dir = 1;
        }
    }
}
