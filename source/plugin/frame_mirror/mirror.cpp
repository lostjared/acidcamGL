#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<MAX> collection;
    
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else if(rand()%4 == 0)
        collection.shiftFrames(frame);
    
    static int off = 0;
    cv::Mat &f = collection.frames[off];

    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = f.at<cv::Vec3b>(z, frame.cols-i-1);
            for(int q = 0; q < 3; ++q)
            pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
        }
    }
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
}
