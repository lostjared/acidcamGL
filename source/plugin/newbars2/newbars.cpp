#include"ac.h"

// off top of my head
extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 32;
    static ac::MatrixCollection<MAX> collection;
    collection.shiftFrames(frame);
    static int off = 0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            for(int q = 0; q < 3; ++q) {
                cv::Mat &m = collection.frames[off];
                cv::Vec3b &pix = m.at<cv::Vec3b>(z, i);
                pixel[q] = static_cast<unsigned char>(pixel[q] * 0.5 + pix[q] * 0.5);
                ++off;
                if(off > MAX-1)
                    off = 0;
            }
        }
    }
}
