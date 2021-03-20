#include"ac.h"


extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<MAX> collection;
    collection.shiftFrames(frame);
    static int off = MAX-1;
    int r = 1+(rand()%64);
    
    for(int z = 0; z < frame.rows; ++z) {
        if((z%r)==0) {
            --off;
            if(off <= 0) {
                off = MAX-1;
            }
        }
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &m = collection.frames[off];
            cv::Vec3b &pix = m.at<cv::Vec3b>(z, i);
            pixel = pix;
            for(int q = 0; q < 3; ++q) {
                pixel[q] = static_cast<unsigned char>((pixel[q] * 0.5) + (pix[q] * 0.5));
            }
        }
    }
}
