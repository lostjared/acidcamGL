#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 4;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    } else
        collection.shiftFrames(frame);

    static int off = 0;
    
    
    for(int z = 0; z < frame.rows; z += 16) {
        
        if(++off > (MAX-1))
            off = 0;
        
        
        
        for(int i = 0; i < frame.cols; i += 16) {
        
            if(++off > (MAX-1))
                off = 0;
            
            if(rand()%2 != 0) continue;
            
            for(int x = 0; x < 16 && i+x < frame.cols; ++x) {
                for(int y = 0; y < 16 && z+y < frame.rows; ++y) {
                    cv::Vec3b &pixel = ac::pixelAt(frame, z+y,i+x);

                    int off_i = AC_GetFX(frame.cols-1, i+x, frame.cols*(off+1));
                    
                    cv::Vec3b &pix = collection.frames[off].at<cv::Vec3b>(z+y, off_i);
                    
                    pixel = pix;
                }
            }
            
        }
    }
}
