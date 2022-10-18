#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    else
    collection.shiftFrames(frame);
    
    static int offset = 0;

    if(++offset > (MAX-1)) {
        offset = 0;
    }

    for(int z = 0; z < frame.rows; z += (offset+1)) {
        for(int i = 0; i < frame.cols; i += (offset+1)) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z, i);
            pixel = pix;
        }
    }
}
