#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<MAX> collection;
    
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else
        collection.shiftFrames(frame);
    
    static int off = 0;
    cv::Mat &f = collection.frames[off];
    static bool on = true;
    static bool on_wait = true;
    
    if(on == true && on_wait == true) {
        for(int z = 0; z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b pix = f.at<cv::Vec3b>(z, i);
                pixel = pix;
            }
        }
    }
    on = !on;
    static int dir = 1;
    if(dir == 1) {
        if(++off > MAX-1) {
            off = MAX-1;
            dir = 0;
        }
    } else {
        if(--off <= 0) {
            off = 0;
            dir = 1;
        }
    }
    
    static int wait = rand()%30;
    static int cnt = 0;
    
    if(++cnt > wait) {
        cnt = 0;
        wait = rand()%30;
        on_wait = !on_wait;
    }
}
