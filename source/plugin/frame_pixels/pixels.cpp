#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<MAX> collection;
    
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else collection.shiftFrames(frame);

    static int off = 0;
    static constexpr int SIZE_W=64;
    
    for(int z = 0; z < frame.rows; z += SIZE_W+1) {
            for(int i = 0; i < frame.cols; i += SIZE_W+1) {
                for(int x = 0; i+x < frame.cols && x < SIZE_W; ++x) {
                    for(int y = 0; z+y < frame.rows && y < SIZE_W; ++y) {
                        
                        cv::Mat &f = collection.frames[off];

                        if(x%8 == 0 || y%8 == 0) {
                            ++off;
                            if(off > MAX-1)
                                off = 0;
                        }
                        
                        cv::Vec3b pix = f.at<cv::Vec3b>(y+z, x+i);
                        cv::Vec3b &pixel = frame.at<cv::Vec3b>(y+z, x+i);
                        pixel = pix;
                    }
            }
        }
    }
}
