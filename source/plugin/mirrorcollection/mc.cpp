#include "ac.h"

extern "C" void filter(cv::Mat &frame) {
    static ac::MatrixCollection<8> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
    }
    collection.shiftFrames(frame);
    static auto callback = [&](cv::Mat *frame, int offset, int cols, int size) {
        int findex = rand()%(collection.size()-1);
        for(int z = offset; z <  offset+size; ++z) {
            for(int i = 0; i < cols; ++i) {
                cv::Vec3b &pixel = frame->at<cv::Vec3b>(z, i);
        
                if((i%50) == 0 && (z%50) == 0) {
                    findex++;
                    if(findex > collection.size()-1)
                        findex = 0;
                }
                
                cv::Mat &m = collection.frames[findex];
                cv::Vec3b &pix = m.at<cv::Vec3b>(m.rows-z-1, m.cols-i-1);
                for(int q = 0; q < 3; ++q) {
                    pixel[q] = static_cast<unsigned char>((0.5 * pixel[q]) + (0.5 * pix[q]));
                }
            }
        }
    };
    ac::UseMultipleThreads(frame, ac::getThreadCount(), callback);
    
}
