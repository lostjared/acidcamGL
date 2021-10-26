#include"ac.h"
#include<cstdlib>
#include<ctime>
#include<cmath>

unsigned char clip_cast(double f) {
    unsigned long x = static_cast<unsigned long>(f);
    if(x > 255)
        x = x%255;
    return static_cast<unsigned char>(x);
}

extern "C" void filter(cv::Mat  &frame) {
    
    static constexpr int MAX = 5;
    int PIXEL_SIZE=4+(rand()%28);
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
    }
    collection.shiftFrames(frame);
    static int off = MAX-1;
    for(int z = 0; z < frame.rows; z += PIXEL_SIZE) {
        for(int i = 0; i < frame.cols; i += PIXEL_SIZE) {
            for(int y = z; y < z+PIXEL_SIZE && y < frame.rows; ++y) {
                for(int x = i; x < i+PIXEL_SIZE && x < frame.cols; ++x) {
                    cv::Vec3b &pixel = frame.at<cv::Vec3b>(y, x);
                    cv::Mat &m = collection.frames[off];
                    cv::Vec3b &pix_off = m.at<cv::Vec3b>(y, x);
                    
                    for(int q = 0; q < 3; ++q) {
                        if(abs(pix_off[q]-pixel[q]) > 50) {
                            pixel = pix_off;
                            break;
                        }
                    }
                    
                }
            }
            PIXEL_SIZE=4+(rand()%28);
            ++off;
            if(off > MAX-1)
                off = 0;
        }
    }
    
    ac::AddInvert(frame);
}
