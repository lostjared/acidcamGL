#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 32;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    else
    collection.shiftFrames(frame);
    static int offset = 0;
    static int wait = rand()%30;
    static int count = 0;

    if(++count > wait) {
        count = 0;
        wait = rand()%30;
        frame = collection.frames[offset].clone();
    } 

    static int dir = 1;
    if(dir == 1) {
        if(++offset > (MAX-1)) {
            offset = rand()%(MAX-1);
            dir = 0;
        }
    } else {
        if(--offset <= 0) {
            offset = rand()%(MAX-1);
            dir = 1;
        }
    }
}
