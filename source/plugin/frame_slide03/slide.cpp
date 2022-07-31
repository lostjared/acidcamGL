#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    collection.shiftFrames(frame);
    
    static int offset_x = 0;
    int offset = rand()%(MAX-1);
    static int offset_width = frame.cols/2;
    
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < offset_width; ++i) {
            if(offset_x+i < frame.cols) {
                cv::Vec3b &pixel = ac::pixelAt(frame, z, offset_x+i);
                cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z, offset_x+i);
                pixel = pix;
            }
        }
    }
    
    static int dir = 1;
    
    if(dir == 1) {
        offset_x += 10;
        if(offset_x > frame.cols-offset_width-1) {
            offset_x = frame.cols-offset_width-1;
            dir = 0;
        }
    } else {
        offset_x -= 10;
        if(offset_x <= 0) {
            offset_x = 0;
            dir = 1;
        }
    }
}
