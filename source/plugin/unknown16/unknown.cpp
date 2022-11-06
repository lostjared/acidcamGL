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
    static int size_x = frame.cols/16;
    static int size_y = frame.rows/16;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            int cx = AC_GetFX(frame.cols-1, i, size_x);
            int cy = AC_GetFZ(frame.rows-1, z, size_y);
            if(cx >= 0 && cx < frame.cols && cy >= 0 && cy < frame.rows) {
                cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(cy, cx);
                pixel = pix;
            }
            
            if(dir == 1) {
                size_x ++;
                size_y ++;
                if(size_x > frame.cols*2)
                    dir = 0;
            } else {
                size_x --;
                size_y --;
                if(size_x <= 0) {
                    dir = 1;
                    size_x = 0;
                }
            }
        }
    }
    
    if(++offset > (MAX-1)) {
        offset = 0;
    }
}
