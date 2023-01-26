#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 4;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    } else collection.shiftFrames(frame);
    
    static int offset = 0;
    static int size_x = 16, size_y = 16;
    
    for(int z = 0; z < frame.rows; z += size_y) {
        for(int i = 0; i < frame.cols; i += size_x) {
            
            if(rand()%3 == 0) continue;
            
            for(int y = 0; y < size_y && y+z < frame.rows; ++y) {
                for(int x = 0; x < size_x && x+i < frame.cols; ++x) {

                    cv::Vec3b &pixel = ac::pixelAt(frame, z+y, i+x);

                    cv::Vec3b &pix = ac::pixelAt(collection.frames[offset], z+y, i+x);
                    
                    pixel = pix;
                }
            }
            offset = rand()%(MAX-1);
        }
    }
    
    if(++size_x > 32) {
        if(++size_y > 32) {
            size_y = 1+(rand()%32);
        }
        size_x = 1+(rand()%32);
    }
}
