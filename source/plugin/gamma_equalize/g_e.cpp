#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static int sw = 1;
    if(sw == 1) {
        ac::FrameSetGamma(frame);
        sw = 0;
    } else {
        ac::Equalize(frame);
        sw = 1;
    }
}
