#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    
    static int counter = 0;
    if(++counter > 8) {
        counter = 0;
        collection.shiftFrames(frame);
    }
    
    static int num = 8;
    static double alpha = 0.2;
    static int offset = 0;
    
    for(int z = 0; z < frame.rows; z += num) {
        for(int i = 0; i < frame.cols; i += num) {
        
            if(rand()%2 == 0) continue;
            
            for(int x = 0; x < num && x+i < frame.cols; ++x) {
                for(int y = 0; y < num && y+z < frame.rows; ++y) {
                
                    
                    cv::Vec3b &pixel = ac::pixelAt(frame, z+y, i+x);
                    cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z+y, i+x);
                    
                    for(int q = 0; q < 3; ++q)
                        pixel[q] = ac::wrap_cast((alpha * pixel[q]) + ((1-alpha)*pix[q]));
                    
                }
            }
        }
    }
    
    if(++offset > (MAX-1)) {
        offset = 0;
    }
    
    static int dir = 1;
    
    if(dir == 1) {
        alpha -= 0.005;
        if(alpha < 0.1)
            dir = 0;
    }
    else {
        alpha += 0.005;
        if(alpha >= 1.0)
            dir = 1;
    }
}
