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
        for(int j = 0; j < rand()%25; ++j) {
            offset = rand()%(MAX-1);
            for(int z = rand()%(frame.rows-1); z < rand()%(frame.rows-1); ++z) {
                for(int i = 0; i < frame.cols; ++i) {
                    cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
                    pixel = ac::pixelAt(collection.frames[7], z, i);
                    
                }
            }
        }
        
    }
    
    strobe = (strobe == true) ? false : true;
}
