#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<2> collection;
    collection.shiftFrames(frame);
    static int offset_x = frame.cols;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; i++) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            int off_x = AC_GetFX(frame.cols-1, i, offset_x);
            offset_x++;
            if(off_x >= 0 && off_x < frame.cols) {
                pixel = collection.frames[1].at<cv::Vec3b>(z, off_x);
            }
        }
        
        if(offset_x > (frame.cols*4)) {
            offset_x = frame.cols;
        }
    }

}
