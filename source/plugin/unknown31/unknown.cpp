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
    int size_y = 0;
    
    size_y = rand()%frame.rows;
    
    for(int z = 0; z < frame.rows/2; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            int cy = AC_GetFZ(frame.rows-1, z, size_y);
            if(cy >= 0 && cy < frame.rows) {
                cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(cy, i);
                pixel = pix;
            }
        }
        size_y ++;
    }
    
    for(int z = frame.rows/2; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            int cy = AC_GetFZ(frame.rows-1, z, size_y);
            
            if(cy >= 0 && cy < frame.rows) {
                cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(cy, i);
                pixel = pix;
            }
        }
        size_y --;
    }
    
    
    if(++offset > (MAX-1)) {
        offset = 0;
    }
    
}
