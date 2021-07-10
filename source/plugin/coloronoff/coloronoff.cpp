#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX_FRAMES=8;
    static ac::MatrixCollection<MAX_FRAMES> collection;
    static int off = 0;
    static int dir = 0;
    
    collection.shiftFrames(frame);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &m = collection.frames[off];
            cv::Vec3b &pix = m.at<cv::Vec3b>(z, i);
            if(dir == 0) {
                if(pixel[0] > 150) {
                    pixel[0] = pix[0] ^ pixel[0];
                }
                if(pixel[1] < 150) {
                    pixel[1] = pix[1] ^ pixel[1];
                }
                if(pixel[2] > 215) {
                    pixel[2] = pix[2] ^ pixel[2];
                }
            } else {
                if(pixel[2] > 150) {
                    pixel[0] = pix[0] ^ pixel[0];
                }
                if(pixel[1] < 150) {
                    pixel[1] = pix[1] ^ pixel[1];
                }
                if(pixel[0] > 215) {
                    pixel[2] = pix[2] ^ pixel[2];
                }
            }
        }
        ++off;
        if(off > MAX_FRAMES-1)
            off = 0;
    }
    if(dir == 0)
        dir = 1;
    else
        dir = 0;
}
