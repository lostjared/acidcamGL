#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<16> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    else if(rand()%3 == 0)
        collection.shiftFrames(frame);
    
    static bool strobe = false;
    static int offset = 0;
    
    if(strobe) {
        
        for(int z = 0; z < frame.rows/4; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
                pixel = ac::pixelAt(collection.frames[offset], z, i);
                
            }
        }
        for(int z = frame.rows/2; z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
                pixel = ac::pixelAt(collection.frames[7], z, i);
                
            }
        }

    }
    
    strobe = (strobe == true) ? false : true;
    static int cnt = 0;
    static int dir = 1;
    if(++cnt > 6) {
        if(dir == 1) {
        ++offset;
            if(offset > MAX-1) {
                dir = 0;
                offset = 2;
            }
            cnt = 0;
        } else {
            --offset;
            if(offset < 0) {
                offset = rand()%(MAX-1);
                dir = 1;
            }
        }
    }
}
