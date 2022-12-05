#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static int wait = rand()%30;
    static int counter = 0;
    if(++counter > wait) {
        wait = rand()%30;
        counter = 0;
        static int sw = 1;
        if(sw == 1) {
            ac::FrameSetGamma(frame);
            sw = 0;
        } else {
            ac::Equalize(frame);
            sw = 1;
        }
    }
}