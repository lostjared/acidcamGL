#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    }
    static int wait = 5;
    static int timeout = 0;
    
    static int offset = 0;
    static int delay = 0;
    static int delay_wait = 3+rand()%3;
    
    if(++timeout > wait) {
        timeout = 0;
        if(++delay > delay_wait) {
            wait = 5+rand()%30;
            delay = 0;
            delay_wait = 3+rand()%3;
        } else {
            wait = 5+rand()%2;
        }
        ++offset;
        if(offset > (MAX-1))
            offset = 0;
        frame = collection.frames[offset].clone();
        
    } else {
        collection.shiftFrames(frame);
        frame = collection.frames[7].clone();
    }
}
