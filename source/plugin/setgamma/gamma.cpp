#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    ac::setGamma(frame, frame, 4);
}
