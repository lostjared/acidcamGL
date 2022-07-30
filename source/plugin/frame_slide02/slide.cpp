#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    collection.shiftFrames(frame);
    static int offset_y = 0;
    static int offset = 0;
    static int offset_height = frame.rows/2;
    for(int z = 0; z < offset_height; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            if(offset_y+z < frame.rows) {
                cv::Vec3b &pixel = ac::pixelAt(frame, offset_y+z, i);
                cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(offset_y+z, i);
                pixel = pix;
            }
        }
    }
    if(++offset > MAX-1)
        offset = 0;
    static int dir = 1;
    if(dir == 1) {
        offset_y += 10;
        if(offset_y > frame.rows-offset_height-1) {
            offset_y = frame.rows-offset_height-1;
            dir = 0;
        }
    } else {
        offset_y -= 10;
        if(offset_y <= 0) {
            offset_y = 0;
            dir = 1;
        }
    }
}
