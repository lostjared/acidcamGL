#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 2;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    
    static int counter = 0;
    if(++counter > 24) {
        counter = 0;
        collection.shiftFrames(frame);
    }
    
    static double alpha = 1.0;
    static int num = 8;
    
    
    for(int z = 0; z < frame.rows; z += num) {
        for(int i = 0; i < frame.cols; i += num) {
        
            if(rand()%3 == 0) continue;
            
            for(int x = 0; x < num && x+i < frame.cols; ++x) {
                for(int y = 0; y < num && y+z < frame.rows; ++y) {
                
                    
                    cv::Vec3b &pixel = ac::pixelAt(frame, z+y, i+x);
                    cv::Vec3b &pix = collection.frames[1].at<cv::Vec3b>(z+y, i+x);
                    
                    for(int q = 0; q < 3; ++q)
                        pixel[q] ^= ac::wrap_cast((alpha*(0.5 * pixel[q]) + (0.5 * pix[q])));
                    
                }
            }
        }
    }
    
    static int dir = 1;
    
    if(dir == 1) {
        alpha += 0.05;
        if(alpha > 5)
            dir = 0;
    }
    else {
        alpha -= 0.05;
        if(alpha <= 1.5)
            dir = 1;
    }
}
