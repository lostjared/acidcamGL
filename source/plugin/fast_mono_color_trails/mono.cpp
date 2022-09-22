#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<16> collection;
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
                    pixel[j] = pix[0];
                }
            }
        }
    };
    ac::UseMultipleThreads(frame, ac::getThreadCount(), callback);
}
