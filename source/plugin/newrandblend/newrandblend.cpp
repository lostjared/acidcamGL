#include"ac.h"


extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<MAX> collection;
    ac::DetectEdges(frame);
    collection.shiftFrames(frame);
    int off = MAX-1;
    int m = 0;
    for(int z = 0; z < frame.rows; ++z) {
        ++m;
        if(m > MAX-1) {
            m = 0;
            off--;
            if(off < 1)
                off = MAX-1;
        }
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &m = collection.frames[off];
            cv::Vec3b &pix = m.at<cv::Vec3b>(z, i);
            pixel = pix;
        }
    }
}
