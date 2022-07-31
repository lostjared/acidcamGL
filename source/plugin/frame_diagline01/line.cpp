#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    collection.shiftFrames(frame);
    
    static int line_start_x = rand()%(frame.cols-1);
    static int line_dip = rand()%50;
    for(int z = 0; z < frame.rows; ++z) {
    
        int line_inc = 0;
        int line_pos = 0;
        
        for(int i = line_start_x; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            ++line_pos;
            if(line_pos > line_dip) {
                ++line_inc;
            }
            
            if(z+line_inc < frame.rows) {
                cv::Vec3b pix = collection.frames[offset].at<cv::Vec3b>(z+line_inc, i);
                pixel = pix;
            }
        }
    }
    
    line_start_x += 100;
    if(line_start_x > frame.cols) {
        line_start_x = rand()%(frame.cols-1);
        line_dip = rand()%50;
    }
}
