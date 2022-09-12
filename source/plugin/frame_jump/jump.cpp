#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    }
    else if (rand()%5 == 0) {
        collection.shiftFrames(frame);
    }
    
    static int offset = 0;
    
    auto drawFrame = [&](int start_h) {
        int new_z = 0;
        for(int z = start_h; z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b pos = collection.frames[offset].at<cv::Vec3b>(new_z, i);
                pixel = pos;
                
            }
            ++new_z;
        }
    };
    static int wait = 0, timeout = 10+rand()%60;
    if(++wait > timeout) {
        wait = 0;
        timeout = 10+rand()%60;
        int new_z = rand()%(frame.cols/2);
        drawFrame(new_z);
        ++offset;
        if(offset > (MAX-1)) {
            offset = 0;
        }
    }
}
