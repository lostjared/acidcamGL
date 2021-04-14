// Just thinking
// first real thoughts
#include"ac.h"

extern "C" void filter(cv::Mat &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    collection.shiftFrames(frame);
    static int off = 0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &m = collection.frames[off];
            cv::Vec3b &v = m.at<cv::Vec3b>(z, i);
            
            // in and out
            // on and off
            
            if((off%4)==0) {
                for(int q = 0; q < 3; ++q) {
                    pixel[q] += pixel[q]^v[q];
                }
            }
            else {
            for(int q = 0; q < 3; ++q)
                pixel[q] += static_cast<unsigned char>(pixel[q] * 0.5 + v[q] * 0.5);
            }
        }
    }
    
    // cycle
    static int r = rand()%10;
    static int c = 0;
    ++c;
    if(c > r) {
        r = rand()%10;
        off++;
        c = 0;
        //give me a break;
        if(off > MAX-1)
            off = 0;
    }
}
