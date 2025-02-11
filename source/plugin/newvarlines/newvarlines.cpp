#include"ac.h"


extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 32;
    static ac::MatrixCollection<MAX> collection;
    collection.shiftFrames(frame);
    int off = 0;
    int shift = rand()%MAX;
    int m = 0;
    for(int z = 0; z < frame.rows; ++z) {
        ++m;
        if(m > shift) {
            ++off;
            m = 0;
            shift = rand()%MAX;
            if(off > MAX-1)
                off = 0;
        }
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &m = collection.frames[off];
            cv::Vec3b &pix = m.at<cv::Vec3b>(z, i);
            pixel = pix;
        }
    }
}
