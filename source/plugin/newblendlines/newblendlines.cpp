#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 128;
    static ac::MatrixCollection<MAX> collection;
    collection.shiftFrames(frame);
    int off = 0;
    int max = rand()%MAX;
    for(int z = 0; z < frame.rows; ++z) {
        ++off;
        if(off >= max)
            off = 0;
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &m = collection.frames[off];
            cv::Vec3b &pix = m.at<cv::Vec3b>(z, i);
            for(int q = 0; q < 3; ++q) {
                pixel[q] = static_cast<unsigned char>((pixel[q] * 0.5) + (pix[q] * 0.5));
            }
        }
    }
}
