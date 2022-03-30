#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 4;
    static ac::MatrixCollection<MAX> collection;
    
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else
        collection.shiftFrames(frame);
    
    static int off = 0;
    
    for(int z = 0; z < frame.rows; z += 34) {
        for(int i = 0; i < frame.cols; i += 34) {
            for(int x = 0; x < 30 && i+x < frame.cols; x++) {
                for(int y = 0; y < 30 && z+y < frame.rows; ++y) {
                    cv::Mat &f = collection.frames[off];
                    cv::Vec3b &pixel = frame.at<cv::Vec3b>(y+z, i+x);
                    cv::Vec3b pix = f.at<cv::Vec3b>(z+y, i+x);
                    pixel = pix;
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
    }
}
