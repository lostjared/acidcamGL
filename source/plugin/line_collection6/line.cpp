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
    
    
    for(int z = 0; z < frame.rows; ++z) {
        
        int cnt = 0;
        int offset = rand()%(MAX-1);
      
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b pix = collection.frames[offset].at<cv::Vec3b>(z, i);
            pixel = pix;
            
            if(++cnt > rand()%50) {
                cnt = 0;
                if(++offset > MAX-1)
                    offset = 0;
            }
        }
    }
}
