#include"ac.h"
#include<cstdlib>
#include<ctime>

extern "C" void filter(cv::Mat  &frame) {

    static constexpr int MAX = 8;
    static int PIXEL_SIZE=8;
    static int dir = 1;
    
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
    }
    collection.shiftFrames(frame);
    
    static int off = 0;
    
    for(int z = 0; z < frame.rows; z += PIXEL_SIZE) {
        for(int i = 0; i < frame.cols; i += PIXEL_SIZE) {
            if((rand()%2)==0) {
                for(int y = z; y < z+PIXEL_SIZE && y < frame.rows; ++y) {
                    for(int x = i; x < i+PIXEL_SIZE && x < frame.cols; ++x) {
                        cv::Vec3b &pixel = frame.at<cv::Vec3b>(y, x);
                        cv::Mat &m = collection.frames[off];
                        cv::Vec3b &pix_off = m.at<cv::Vec3b>(y, x);
                        pixel = pix_off;
                        
                    }
                }
            }
        }
        ++off;
        if(off > MAX-1)
            off = 0;
    }
    
    if(dir == 1) {
        PIXEL_SIZE ++;
        if(PIXEL_SIZE >= 32)
            dir = 0;
    } else {
        PIXEL_SIZE --;
        if(PIXEL_SIZE <= 8)
            dir = 1;
    }
    ac::AddInvert(frame);
}
