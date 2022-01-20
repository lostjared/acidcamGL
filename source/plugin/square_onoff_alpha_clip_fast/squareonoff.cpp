#include"ac.h"
#include<cstdlib>
#include<ctime>

unsigned char clip_cast(double f) {
    unsigned long x = static_cast<unsigned long>(f);
    if(x > 255)
        x = x%255;
    return static_cast<unsigned char>(x);
}

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    int PIXEL_SIZE=4+(rand()%36);
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
    }
    collection.shiftFrames(frame);
    static int off = 0;
    static double alpha = 0.1;
    for(int z = 0; z < frame.rows; z += PIXEL_SIZE) {
        for(int i = 0; i < frame.cols; i += PIXEL_SIZE) {
            if((rand()%2)==0) {
                for(int y = z; y < z+PIXEL_SIZE && y < frame.rows; ++y) {
                    for(int x = i; x < i+PIXEL_SIZE && x < frame.cols; ++x) {
                        cv::Vec3b &pixel = frame.at<cv::Vec3b>(y, x);
                        cv::Mat &m = collection.frames[off];
                        cv::Vec3b &pix_off = m.at<cv::Vec3b>(y, x);
                        for(int q = 0; q < 3; ++q) {
                            pixel[q] = clip_cast((alpha*pixel[q])+((1-alpha)*pix_off[q]));
                        }
                    }
                }
            }
        }
        static int adir = 1;
        if(adir == 1) {
            alpha += 0.07;
            if(alpha >= 8.0)
                adir = 0;
        } else {
            alpha -= 0.07;
            if(alpha <= 0.1)
                adir = 1;
        }
        ++off;
        if(off > MAX-1)
            off = 0;
    }
    
    ac::AddInvert(frame);
}
