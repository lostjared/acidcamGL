#include "ac.h"

extern "C" void filter(cv::Mat &frame) {
    static ac::MatrixCollection<8> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
    }
    collection.shiftFrames(frame);
    
    static int size_x = 24, size_y = 24;
    
    static auto callback = [&](cv::Mat *frame, int offset, int cols, int size) {
        static int findex = 0;
        for(int z = offset; z <  offset+size; ++z) {
            for(int i = 0; i < cols; ++i) {
                cv::Vec3b &pixel = frame->at<cv::Vec3b>(z, i);
        
                if((i%size_x) == 0 && (z%size_y) == 0) {
                    findex++;
                    if(findex > collection.size()-1)
                        findex = 0;
                }
                
                cv::Mat &m = collection.frames[findex];
                cv::Vec3b &pix = m.at<cv::Vec3b>(z, i);
                pixel = pix;
            }
        }
    };
    ac::UseMultipleThreads(frame, ac::getThreadCount(), callback);
    
    static int dir = 1;
    if(dir == 1) {
        size_x += 2;
        size_y += 2;
        
        if(size_x > frame.cols/2) {
            dir = 0;
        }
    } else {
        size_x -= 2;
        size_y -= 2;
        if(size_x <= 2) {
            dir = 1;
        }
    }
    
}
