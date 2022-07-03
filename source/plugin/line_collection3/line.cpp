#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    collection.shiftFrames(frame);
    
    
    for(int z = 0; z < frame.rows; ++z) {
        
        int cnt = 0;
        int offset = 0;
      
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b pix = collection.frames[offset].at<cv::Vec3b>(z, i);
            pixel = pix;
            
            if(++cnt > 2) {
                cnt = 0;
                if(++offset > MAX-1)
                    offset = 0;
            }
        }
    }
}
