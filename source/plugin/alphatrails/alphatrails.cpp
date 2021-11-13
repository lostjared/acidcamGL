#include"ac.h"
#include<cstdlib>
#include<ctime>

unsigned char wrap_cast(double f) {
    unsigned long x = static_cast<unsigned long>(f);
    if(x > 255)
        x = x%255;
    return static_cast<unsigned char>(x);
}

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<MAX> collection;
    collection.shiftFrames(frame);
    cv::Mat *frames[3];
    frames[0] = &collection.frames[1];
    frames[1] = &collection.frames[7];
    frames[2] = &collection.frames[15];
    auto callback = [&](cv::Mat *frame, int offset, int cols, int size) {
        for(int z = offset; z <  offset+size; ++z) {
            for(int i = 0; i < cols; ++i) {
                cv::Vec3b &pixel = frame->at<cv::Vec3b>(z, i);
                for(int j = 0; j < 3; ++j) {
                    cv::Vec3b pix = frames[j]->at<cv::Vec3b>(z, i);
                    pixel[j] = wrap_cast((0.7 * pixel[j]) + (0.3 * pix[0]));
                }
            }
        }
    };
    ac::UseMultipleThreads(frame, ac::getThreadCount(), callback);
}
