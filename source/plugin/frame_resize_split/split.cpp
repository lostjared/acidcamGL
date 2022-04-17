#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<MAX> collection;
    
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else
        collection.shiftFrames(frame);
    
    int frame_size = 8;
    int frame_x_off = 0;
    static int off = 0;
    for(int z = 0; z < frame.rows; z++) {
        if(z%frame_size == 0) {
            ++off;
            if(off > MAX-1)
                off = 0;
            frame_x_off = rand()%frame_size;
        }
        int pos = 0;
        for(int i = frame_x_off; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, pos);
            cv::Mat &m = collection.frames[off];
            pixel = m.at<cv::Vec3b>(z, i);
            ++pos;
        }
    }
}
