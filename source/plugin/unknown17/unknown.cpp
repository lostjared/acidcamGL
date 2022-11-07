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
    static int dir = 1;
    static int offset = 0;
    static int div = 2;
    static int size_y = frame.rows/16;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            int cy = AC_GetFZ(frame.rows-1, z, size_y);
            if(cy >= 0 && cy < frame.rows && i >= 0 && i < frame.cols) {
                cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(cy, i);
                pixel = pix;
            }
        }
        size_y ++;
        if(size_y > frame.rows*2)
            size_y = rand()%(frame.rows-1);
    }
    
    if(++offset > (MAX-1)) {
        offset = 0;
    }
}
