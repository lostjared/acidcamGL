#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 4;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    static int counter = 0;
    if(++counter > 4) {
        counter = 0;
        collection.shiftFrames(frame);
    }
    static int offset = 0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b &pix = ac::pixelAt(collection.frames[offset], z, i);
            pixel = pix;
        }
    }
    if(++offset > (MAX-1)) {
        offset = 0;
    }
}
