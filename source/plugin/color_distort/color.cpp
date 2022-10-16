#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 3;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    else
    collection.shiftFrames(frame);
    
    static int pos = 0;
    static int offset = 0;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z, i);
            switch(pos) {
                case 0:
                    pixel[0] ^= pix[1];
                    pixel[1] &= pix[2];
                    pixel[2] |= pix[0];
                    break;
                case 1:
                    pixel[0] &= pix[2];
                    pixel[1] ^= pix[0];
                    pixel[2] |= pix[1];
                    break;
                case 2:
                    pixel[0] |= pix[0];
                    pixel[1] &= pix[1];
                    pixel[2] ^= pix[2];
                    break;
            }
        }
    }
    
    if(++pos > 2) {
        pos = 0;
    }
    
    if(++offset > (MAX-1)) {
        offset = 0;
    }
}
