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
    
    static int offset = 0;
    static int num = 1+(rand()%100);
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z, i);
            pixel = pix;
        }
        if((z%num) == 0) {
            num = 1+(rand()%250);
            static int dir = 1;
            if(dir == 1) {
                if(++offset > (MAX-1)) {
                    offset = MAX-1;
                    dir = 0;
                }
            } else {
                if(--offset <= 0) {
                    offset = 0;
                    dir = 1;
                }
            }
        }
    }
}
