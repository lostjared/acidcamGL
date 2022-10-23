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
    
    static int offset = 0;
    static int nw = frame.cols;
    static int nh = frame.rows;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            int offx = AC_GetFX(frame.cols-1, i, nw);
            int offy = AC_GetFZ(frame.rows-1, z, nh);
            cv::Vec3b &cpix = collection.frames[offset].at<cv::Vec3b>(offy, offx);
            
            pixel = cpix;
        }
        
        if((rand()%10) == 0) {
            nw += 1;
            nh += 1+(rand()%5);
        }
    }
    
    if(nw > frame.cols+25) {
        nw = frame.cols;
    }
    if(nh > frame.rows+25) {
        nh = frame.rows;
    }
    
    if(++offset > (MAX-1)) {
        offset = 0;
    }
}

