#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<32> collection;
    collection.shiftFrames(frame);
    static int off = 0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &f = collection.frames[off];
            cv::Vec3b pix = f.at<cv::Vec3b>(z, i);
            pixel = pix;
            if(rand()%25 == 0) {
                ++off;
                if(off > 31) {
                    off = 0;
                }
            }
        }
    }
}