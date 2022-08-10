#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    collection.shiftFrames(frame);
   
    int cy_start = rand()%(frame.rows-1);
    int cy_stop = cy_start+(rand()%frame.rows/6);
    
    int offset = rand()%(MAX-1);
    int off = rand()%3;
    
    for(int z = cy_start; z < frame.rows && z < cy_stop; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b cpix = collection.frames[offset].at<cv::Vec3b>(z, i);
             pixel = cpix;
        }
    }
}
