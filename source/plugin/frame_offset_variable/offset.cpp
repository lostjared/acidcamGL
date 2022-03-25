#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static bool started = false;
    
    if(started == false) {
        srand(static_cast<unsigned int>(time(0)));
        started = true;
    }
    
    cv::Mat copy = frame.clone();
    static constexpr int SIZE=32;
    static int MAX = SIZE;
    int start_offset = 1+(rand()%SIZE);
    static bool on = true;
    
    if(on == true) {
        for(int z = 0; z < frame.rows; ++z) {
            int f_offset = 0;
            for(int i = start_offset; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b pix = copy.at<cv::Vec3b>(z, f_offset);
                pixel = pix;
                f_offset ++;
            }
            static int inc_val = rand()%MAX;
            ++start_offset;
            if(start_offset > inc_val) {
                start_offset = inc_val;
                MAX = 1+(rand()%SIZE);
                inc_val = rand()%MAX;
            }
        }
    }
    static int wait = rand()%24;
    static int cnt = 0;
    
    if(++cnt > wait) {
        cnt = 0;
        wait = rand()%24;
        on = !on;
    }
    
}
