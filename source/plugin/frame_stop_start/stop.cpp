#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<MAX> collection;
    static int max = 20;
    
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else if(rand()%max == 0) {
        collection.shiftFrames(frame);
        max = 1+(rand()%30);
    }
    
    static int off = 0;
    cv::Mat &f = collection.frames[off];
    
    static auto callback = [&](cv::Mat *frame, int offset, int cols, int size) {
        for(int z = offset; z <  offset+size; ++z) {
            for(int i = 0; i < cols; ++i) {
                cv::Vec3b &pixel = frame->at<cv::Vec3b>(z, i);
                cv::Vec3b pix = f.at<cv::Vec3b>(z, i);
                pixel[0] = ac::wrap_cast ((0.5 * pixel[0]) + (0.5 * pix[0]));
                
                pixel[1] = ac::wrap_cast ((0.5 * pixel[1]) + (0.5 * pix[1]));
                
                pixel[2] = ac::wrap_cast ((0.5 * pixel[2]) + (0.5 * pix[2]));
            }
        }
    };
    ac::UseMultipleThreads(frame, ac::getThreadCount(), callback);
    
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
