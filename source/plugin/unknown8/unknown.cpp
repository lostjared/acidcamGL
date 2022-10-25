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
    
    static int dir = 1;
    static int offset = 0;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z, i);
            
            for(int q = 0; q < 3; ++q) {
                pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
            }
            
        }
    }
 
    if(dir == 1) {
        static int wait = 0;
        static int timeout = rand()%10;
        
        if(++offset > (MAX-1)) {
            offset = 0;
        }
        
        if(++wait > timeout) {
            wait = 0;
            timeout = rand()%10;
            dir = 0;
        }
    } else {
        static int wait = 0;
        static int timeout = rand()%10;
        if(--offset <= 0) {
            offset = MAX-1;
        }

        if(++wait > timeout) {
            wait = 0;
            timeout = rand()%10;
            dir = 1;
        }
    }
}
