#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else if(rand()%5 == 0) {
        collection.shiftFrames(frame);
    }
    
    static int wait = 0;
    static int timeout = 10+rand()%24;
    
    if(++wait > timeout) {
        wait = 0;
        timeout = 10+rand()%24;
        
        int new_h = frame.rows+(rand()%(frame.rows/2));
        int new_w = frame.cols+(rand()%(frame.cols/2));
        
        for(int z = 0; z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                int nz = AC_GetFZ(frame.rows-1, z, new_h);
                int ni = AC_GetFX(frame.cols-1, i, new_w);
                if(nz > 0 && ni > 0 && nz < frame.rows && ni < frame.cols) {
                    cv::Vec3b pix = collection.frames[offset].at<cv::Vec3b>(nz, ni);
                    pixel = pix;
                }
            }
        }
    }
    
}
